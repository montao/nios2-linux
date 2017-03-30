#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Copyright (C) 2005,2008 Insecure.Com LLC.
#
# Author: Adriano Monteiro Marques <py.adriano@gmail.com>
# Modified: Jurand Nogiec <jurand@jurand.net>, 2008
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

import errno
import gtk
import gobject
import os
import re
import xml.sax

from zenmapGUI.higwidgets.hignotebooks import HIGNotebook, HIGAnimatedTabLabel
from zenmapGUI.higwidgets.higboxes import HIGVBox
from zenmapGUI.higwidgets.higdialogs import HIGAlertDialog, HIGDialog
from zenmapGUI.higwidgets.higscrollers import HIGScrolledWindow

from zenmapGUI.ScanHostDetailsPage import ScanHostDetailsPage
from zenmapGUI.ScanToolbar import ScanCommandToolbar, ScanToolbar
from zenmapGUI.ScanHostsView import ScanHostsView, SCANNING, CANCELLED
from zenmapGUI.ScanOpenPortsPage import ScanOpenPortsPage
from zenmapGUI.ScanRunDetailsPage import ScanRunDetailsPage
from zenmapGUI.ScanNmapOutputPage import ScanNmapOutputPage
from zenmapGUI.ScanScanListPage import ScanScanListPage
from zenmapGUI.ScansListStore import ScansListStore
from zenmapGUI.TopologyPage import TopologyPage
from zenmapGUI.Icons import get_os_icon, get_os_logo, get_vulnerability_logo

from zenmapCore.NetworkInventory import NetworkInventory
from zenmapCore.NmapCommand import NmapCommand
from zenmapCore.UmitConf import CommandProfile, ProfileNotFound, is_maemo
from zenmapCore.NmapParser import NmapParser
from zenmapCore.Paths import Path, get_extra_executable_search_paths
from zenmapCore.UmitLogging import log
from zenmapCore.NmapOptions import NmapOptions
import zenmapCore.I18N

class ScanInterface(HIGVBox):
    """ScanInterface contains the scan toolbar and the scan results. Each
    ScanInterface represents a single NetworkInventory as well as a set of
    running scans."""
    def __init__(self):
        HIGVBox.__init__(self)

        # The borders are consuming too much space on Maemo. Setting it to
        # 0 pixels while on Maemo
        if is_maemo():
            self.set_border_width(0)
        
        self.set_spacing(0)

        # True if nothing has happened here page yet, i.e., it's okay to load a
        # scan from a file here.
        self.empty = True

        # The most recent name the inventory on this page has been saved under.
        self.saved_filename = None
        
        # The network inventory shown by this page. It may consist of multiple
        # scans.
        self.inventory = NetworkInventory()

        # The list of currently running scans (NmapCommand objects).
        self.jobs = []

        # The list of running and finished scans shown on the Nmap Output page.
        self.scans_store = ScansListStore()

        self.top_box = HIGVBox()
        
        self.__create_toolbar()
        self.__create_command_toolbar()

        self.select_default_profile()

        self.scan_result = ScanResult(self.inventory, self.scans_store)
        self.host_view_selection = self.scan_result.get_host_selection()
        self.service_view_selection = self.scan_result.get_service_selection()
        self.host_view_selection.connect('changed', self.update_host_info)
        self.service_view_selection.connect('changed', self.update_service_info)

        self.scan_result.scan_result_notebook.nmap_output.connect("changed", self._displayed_scan_change_cb)
        self.scan_result.scan_result_notebook.scans_list.remove_button.connect("clicked", self._remove_scan_cb)

        self.hosts = {}
        self.services = {}

        self.top_box.set_border_width(6)
        self.top_box.set_spacing(5)
        
        self.top_box._pack_noexpand_nofill(self.toolbar)
        self.top_box._pack_noexpand_nofill(self.command_toolbar)
        
        self._pack_noexpand_nofill(self.top_box)
        self._pack_expand_fill(self.scan_result)

        self.scan_result.scan_result_notebook.scans_list.cancel_button.connect("clicked", self._cancel_scans_list_cb)
        self.update_cancel_button()

    def is_changed(self):
        """Return true if this window has unsaved changes."""
        for scan in self.inventory.get_scans():
            if scan.unsaved:
                return True
        return False
    changed = property(is_changed)

    def num_scans_running(self):
        return len(self.jobs)

    def select_default_profile(self):
        """Select a "default" profile. Currently this is defined to the first
        profile."""
        if len(self.toolbar.profile_entry.get_model()) > 0:
            self.toolbar.profile_entry.set_active(0)

    def go_to_host(self, host):
        """Go to host line on nmap output result"""
        self.scan_result.scan_result_notebook.nmap_output.nmap_output.go_to_host(host)

    def __create_toolbar(self):
        self.toolbar = ScanToolbar()
        
        self.target_entry_changed_handler = \
            self.toolbar.target_entry.connect('changed', self._target_entry_changed)
        self.profile_entry_changed_handler = \
            self.toolbar.profile_entry.connect('changed', self._profile_entry_changed)

        self.toolbar.scan_button.connect('clicked', self.start_scan_cb)
        self.toolbar.cancel_button.connect('clicked', self._cancel_scan_cb)

    
    def __create_command_toolbar(self):
        """Assigns several events to the command toolbar, including activate
        on scan button being clicked."""

        self.command_toolbar = ScanCommandToolbar()
        self.command_toolbar.command_entry.connect('activate',
                                    lambda x: self.toolbar.scan_button.clicked())

        self.command_entry_changed_handler = \
            self.command_toolbar.command_entry.connect('changed', self._command_entry_changed)

    def _command_entry_changed(self, editable):
        command_string = self.command_toolbar.get_command()
        command_list = command_string.split()
        ops = NmapOptions()
        ops.parse(command_list[1:])

        # Set the target and profile without propagating the "changed" signal
        # back to the command entry.
        self.set_target_quiet(" ".join(ops.target_specs))
        self.set_profile_name_quiet("")

    def _target_entry_changed(self, editable):
        target_string = self.toolbar.get_selected_target()
        targets = target_string.split()

        command_string = self.command_toolbar.get_command()
        command_list = command_string.split()
        if len(command_list) == 0:
            command_list = ["nmap"]
        ops = NmapOptions()
        ops.parse(command_list[1:])
        ops.target_specs = targets

        command_string = command_list[0] + " " + ops.render_string()

        self.set_command_quiet(command_string)

    def _profile_entry_changed(self, widget):
        """Update the command based on the contents of the target and profile
        entries. If the command corresponding to the current profile is not
        blank, use it. Otherwise use the current contents of the command
        entry."""
        profile_name = self.toolbar.get_selected_profile()
        target_string = self.toolbar.get_selected_target()

        cmd_profile = CommandProfile()
        command_string = cmd_profile.get_command(profile_name)
        del(cmd_profile)
        if command_string == "":
            command_string = self.command_toolbar.get_command()

        command_list = command_string.split()
        if len(command_list) == 0:
            command_list = ["nmap"]
        ops = NmapOptions()
        ops.parse(command_list[1:])

        # Use the targets from the command entry, if there are any, otherwise
        # use any targets from the profile.
        targets = target_string.split()
        if len(targets) > 0:
            ops.target_specs = targets
        else:
            self.toolbar.set_selected_target(" ".join(ops.target_specs))

        command_string = command_list[0] + " " + ops.render_string()

        self.set_command_quiet(command_string)

    def set_command_quiet(self, command_string):
        """Set the command used by this scan interface, ignoring any further
        "changed" signals."""
        self.command_toolbar.command_entry.handler_block(self.command_entry_changed_handler)
        self.command_toolbar.set_command(command_string)
        self.command_toolbar.command_entry.handler_unblock(self.command_entry_changed_handler)

    def set_target_quiet(self, target_string):
        """Set the target string used by this scan interface, ignoring any
        further "changed" signals."""
        self.toolbar.target_entry.handler_block(self.target_entry_changed_handler)
        self.toolbar.set_selected_target(target_string)
        self.toolbar.target_entry.handler_unblock(self.target_entry_changed_handler)

    def set_profile_name_quiet(self, profile_name):
        """Set the profile name used by this scan interface, ignoring any
        further "changed" signals."""
        self.toolbar.profile_entry.handler_block(self.profile_entry_changed_handler)
        self.toolbar.set_selected_profile(profile_name)
        self.toolbar.profile_entry.handler_unblock(self.profile_entry_changed_handler)

    def start_scan_cb(self, widget=None):
        target = self.toolbar.selected_target
        command = self.command_toolbar.command
        profile = self.toolbar.selected_profile

        log.debug(">>> Start Scan:")
        log.debug(">>> Target: '%s'" % target)
        log.debug(">>> Profile: '%s'" % profile)
        log.debug(">>> Command: '%s'" % command)

        ##### If target empty, we are not changing the command
        if target != '':    
            self.toolbar.add_new_target(target)

        if command == '':
            warn_dialog = HIGAlertDialog(message_format=_("Empty Nmap Command"),
                                         secondary_text=_("There is no command to  \
execute. Maybe the selected/typed profile doesn't exist. Please, check the profile name \
or type the nmap command you would like to execute."),
                                         type=gtk.MESSAGE_ERROR)
            warn_dialog.run()
            warn_dialog.destroy()
            return

        self.execute_command(command, target, profile)

    def _displayed_scan_change_cb(self, widget):
        self.update_cancel_button()

    def update_cancel_button(self):
        """Make the Cancel button sensitive or not depending on whether the
        currently displayed scan is running."""
        entry = self.scan_result.scan_result_notebook.nmap_output.get_active_entry()
        if entry is None:
            self.toolbar.cancel_button.set_sensitive(False)
        else:
            self.toolbar.cancel_button.set_sensitive(entry.running)

    def _cancel_scan_cb(self, widget):
        """Cancel the scan whose output is shown."""
        entry = self.scan_result.scan_result_notebook.nmap_output.get_active_entry()
        if entry is not None and entry.running:
            self.cancel_scan(entry.command)

    def _cancel_scans_list_cb(self, widget):
        """This is like _cancel_scan_cb, but it cancels the scans that are
        currently selected in the scans list, not the one whose output is
        currently shown."""
        model, selection = self.scan_result.scan_result_notebook.scans_list.scans_list.get_selection().get_selected_rows()
        for path in selection:
            entry = model.get_value(model.get_iter(path), 0)
            if entry.running:
                self.cancel_scan(entry.command)

    def _remove_scan_cb(self, widget):
        model, selection = self.scan_result.scan_result_notebook.scans_list.scans_list.get_selection().get_selected_rows()
        selected_refs = []
        for path in selection:
            # Kill running scans and remove finished scans from the inventory.
            entry = model.get_value(model.get_iter(path), 0)
            if entry.running:
                self.cancel_scan(entry.command)
            try:
                # Remove it from the inentory if present.
                self.inventory.remove_scan(entry.parsed)
            except ValueError:
                pass
            # Create TreeRowReferences because those persist while we change the
            # model.
            selected_refs.append(gtk.TreeRowReference(model, path))
        # Delete the entries from the ScansListStore.
        for ref in selected_refs:
            model.remove(model.get_iter(ref.get_path()))
        self.update_ui()

    def collect_umit_info(self, command, parsed):
        profile = CommandProfile()
        profile_name = command.profile
        
        parsed.target = command.target
        parsed.profile_name = profile_name
        parsed.nmap_command = command.command

        del(profile)

    def kill_all_scans(self):
        """Kill all running scans."""

        for scan in self.jobs:
            try:
                scan.kill()
            except AttributeError:
                pass
        del self.jobs[:]

    def cancel_scan(self, command):
        """Cancel a running scan."""
        self.scans_store.cancel_running_scan(command)
        command.kill()
        self.jobs.remove(command)
        self.update_cancel_button()

    def execute_command(self, command, target = None, profile = None):
        """If scan state is alive and user responds OK, stop the currently active scan
        and allow creation of another, and if user responds Cancel, wait the current scan to finish.
        Invokes NmapCommand for execution. Verifies if a valid nmap executable exists in PATH, if not,
        displays error with the offending PATH. Refreshes and changes to nmap output view from given file."""
        command_execution = NmapCommand(command)
        command_execution.target = target
        command_execution.profile = profile
        
        try:
            command_execution.run_scan()
        except Exception, e:
            text = str(e)
            if type(e) == OSError:
                # Handle ENOENT specially.
                if e.errno == errno.ENOENT:
                    # nmap_command_path comes from zenmapCore.NmapCommand.
                    text += "\n\n" + _("This means that the nmap executable was not found in your system PATH, which is") + "\n\n" + os.getenv("PATH", _("<undefined>"))
                    path_env = os.getenv("PATH")
                    if path_env is None:
                        default_paths = []
                    else:
                        default_paths = path_env.split(os.pathsep)
                    extra_paths = get_extra_executable_search_paths()
                    extra_paths = [p for p in extra_paths if p not in default_paths]
                    if len(extra_paths) > 0:
                        if len(extra_paths) == 1:
                            text += "\n\n" + _("plus the extra directory")
                        else:
                            text += "\n\n" + _("plus the extra directories")
                        text += "\n\n" + os.pathsep.join(extra_paths)
            warn_dialog = HIGAlertDialog(message_format=_("Error executing command"),
                secondary_text=text, type=gtk.MESSAGE_ERROR)
            warn_dialog.run()
            warn_dialog.destroy()
            return

        log.debug("Running command: %s" % command_execution.command)
        self.jobs.append(command_execution)

        i = self.scans_store.add_running_scan(command_execution)
        self.scan_result.scan_result_notebook.nmap_output.set_active_iter(i)

        # When scan starts, change to nmap output view tab and refresh output
        self.scan_result.change_to_nmap_output_tab()
        self.scan_result.refresh_nmap_output()

        # Add a timeout function
        self.verify_thread_timeout_id = gobject.timeout_add(2000, self.verify_execution)

    def verify_execution(self):
        """To verify execution, get the scan_state of the command execution. If it errors out,
        disable the widgets and set the scan as failed. Automatically refreshes the nmap output. Returns
        True if execution verified, False otherwise"""

        self.scan_result.refresh_nmap_output()
        
        finished_jobs = []
        for scan in self.jobs:
            try:
                alive = scan.scan_state()
                if alive:
                    continue
            except:
                log.debug("Scan terminated unexpectedly: %s" % scan.command)
                self.scans_store.fail_running_scan(scan)
            else:
                log.debug("Scan finished: %s" % scan.command)
                self.load_from_command(scan)
                scan.close()
            self.update_cancel_button()
            finished_jobs.append(scan)

        # Remove finished jobs from the job list
        for finished in finished_jobs:
            self.jobs.remove(finished)
        del(finished_jobs)

        return len(self.jobs) != 0

    def load_from_command(self, command):
        """Load scan results from a completed NmapCommand."""
        parsed = None
        try:
            parsed = self._parse(command.get_xml_output_filename())
        except IOError, e:
            # It's possible to run Nmap without generating an XML output file,
            # like with "nmap -V".
            if e.errno != errno.ENOENT:
                raise
        except xml.sax.SAXParseException, e:
            try:
                # Some options like --iflist cause Nmap to emit an empty XML
                # file. Ignore the exception in this case.
                st = os.stat(command.get_xml_output_filename())
            except:
                st = None
            if st is None or st.st_size > 0:
                warn_dialog = HIGAlertDialog(message_format = _("Parse error"),
                    secondary_text = _(u"""\
There was an error while parsing the XML file generated from the scan:

%s\
""") % str(e), type = gtk.MESSAGE_ERROR)
                warn_dialog.run()
                warn_dialog.destroy()
        else:
            parsed.unsaved = True

            parsed.set_xml_is_temp(command.xml_is_temp)
            self.collect_umit_info(command, parsed)
            parsed.nmap_output = command.get_output()
            self.scan_result.refresh_nmap_output()

            self.inventory.add_scan(parsed)
        self.update_ui()
        self.scans_store.finish_running_scan(command, parsed)

    def load_from_file(self, filename):
        """Load scan results from a saved file."""
        parsed = self._parse(filename)
        parsed.unsaved = False

        self.update_target_profile(parsed)
        self.inventory.add_scan(parsed, filename=filename)
        self.update_ui()
        i = self.scans_store.add_scan(parsed)
        self.scan_result.scan_result_notebook.nmap_output.set_active_iter(i)
        self.scan_result.change_to_ports_hosts_tab()

    def load_from_parsed_result(self, parsed_result):
        """Load scan results from a parsed NmapParser object."""
        parsed = parsed_result
        parsed.unsaved = False

        self.update_target_profile(parsed)
        self.inventory.add_scan(parsed)
        self.update_ui()
        i = self.scans_store.add_scan(parsed)
        self.scan_result.scan_result_notebook.nmap_output.set_active_iter(i)
        self.scan_result.change_to_ports_hosts_tab()

    def _parse(self, file_to_parse):
        """Parse the given file and return a new NmapParser object. Display an
        error dialog if there is an error in parsing."""
        parsed = NmapParser()
        log.debug(">>> XML output file that is going to be parsed: %s" % file_to_parse)
        log.debug(">>> Start parsing...")
        parsed.parse_file(file_to_parse)
        log.debug(">>> Successfully parsed!")

        return parsed

    def update_target_profile(self, parsed):
        """Update the "Target" and "Profile" entries based on the contents of a
        parsed scan."""
        command = parsed.get_nmap_command()
        target = parsed.get_target()
        profile_name = parsed.get_profile_name()

        self.set_command_quiet(parsed.get_nmap_command() or "")
        self.set_target_quiet(target or "")
        self.set_profile_name_quiet(profile_name or "")
        
    def update_ui(self):
        """Update the interface's lists of hosts and ports from a parsed
        scan."""
        self.empty = False

        self.scan_result.scan_host_view.clear_host_list()
        self.scan_result.scan_host_view.clear_service_list()

        self.scan_result.scan_result_notebook.topology.update_radialnet()

        self.hosts = {}
        self.services = {}

        for host in self.inventory.get_hosts():
            hostname = host.get_hostname()
            host_page = self.set_host_details(host)

            for service in host.services:
                name = service["service_name"]
                state = service["port_state"]

                if state not in ["open", "filtered", "open|filtered"]:
                    continue

                if name not in self.services.keys():
                    self.services[name] = {"hosts":[]}

                hs = {"host":host, "page":host_page, "hostname":hostname}
                hs.update(service)
                    
                self.services[name]["hosts"].append(hs)

            self.hosts[hostname] = {'host':host, 'page':host_page}
                
            host_details = self.hosts[hostname]['page'].host_details
            host_info = self.hosts[hostname]['host']

            try:
                host_details.set_os_image(get_os_logo(host))
            except:
                host_details.set_os_image(get_os_logo(''))
                
            host_details.set_vulnerability_image(get_vulnerability_logo\
                                                 (host_info.get_open_ports()))
                
            try:
                icon = get_os_icon(host)
            except:
                icon = get_os_icon('')
                
            self.scan_result.scan_host_view.add_host({hostname:{'stock':icon,
                                                                'action':None}})
                
        if len(self.scan_result.scan_host_view.host_list) > 0:
            # Select the first host found
            self.host_view_selection.select_iter(self.scan_result.scan_host_view.host_list.get_iter_root())

        self.scan_result.scan_host_view.set_services(self.services.keys())

    def update_host_info(self, widget):
        """"""

        self.scan_result.scan_result_notebook.port_mode()

        model_host_list, selection = widget.get_selected_rows()
        #host_objs = [self.hosts[model_host_list[i[0]][1]] for i in selection]
        host_objs = []
        for i in selection:
            key = model_host_list[i[0]][1]
            if self.hosts.has_key(key):
                host_objs.append(self.hosts[key])

        self.clean_host_details()
        
        if len(host_objs) == 1:
            self.set_single_host_port(host_objs[0]['host'])
            self.switch_host_details(host_objs[0]['page'])
        else:
            self.set_multiple_host_port(host_objs)
            self.switch_host_details(self.set_multiple_host_details(host_objs))

        # Switch nmap output to show first host occourrence
        try:
            self.go_to_host(host_objs[0]['host'].get_hostname())
        except IndexError:
            pass

    def update_service_info(self, widget):
        """If a service information in a particular widget has changed, it will update it."""

        self.scan_result.scan_result_notebook.host_mode()
        
        model_service_list, selection = widget.get_selected_rows()
        #serv_objs = [self.services[model_service_list[i[0]][0]] for i in selection]
        serv_objs = []
        for i in selection:
            key = model_service_list[i[0]][0]
            if self.services.has_key(key):
                serv_objs.append(self.services[key])

        # Removing current widgets from the host details page
        self.clean_host_details()

        if len(serv_objs) == 1:
            self.set_single_service_host(serv_objs[0]['hosts'])
            self.switch_host_details([host["page"] for host in serv_objs[0]['hosts']])
        else:
            servs = []
            for s in serv_objs:
                servs.append({"service_name":s["hosts"][0]["service_name"],
                     "hosts":s["hosts"]})

            self.set_multiple_service_host(servs)
            
            pages = []
            for serv in [serv["hosts"] for serv in serv_objs]:
                for h in serv:
                    # Prevent from adding a host more then once
                    if h["page"] not in pages:
                        pages.append(h["page"])
            
            self.switch_host_details(pages)

        # Change scan tab to "Ports/Hosts"
        self.scan_result.change_to_ports_hosts_tab()
    
    def clean_host_details(self):
        parent = self.scan_result.scan_result_notebook.host_details_vbox
        children = parent.get_children()
        
        for child in children:
            parent.remove(child)
            
    def switch_host_details(self, page):
        """To switch host details, check the length of the page. If there is multiple pages,
        set them hidden and not expanded. If there is single page, set that page to the first page."""

        if type(page) == type([]):
            if len(page) > 1:
                for p in page:
                    p.hide()
                    p.set_expanded(False)
                    if p not in self.scan_result.scan_result_notebook.host_details_vbox:
                        self.scan_result.scan_result_notebook.host_details_vbox._pack_noexpand_nofill(p)
                
                self.scan_result.scan_result_notebook.host_details_vbox.show_all()
                
                return
            elif len(page) == 1:
                page = page[0]
        
        try:
            page.hide()
        except:
            pass
        else:
            self.scan_result.scan_result_notebook.host_details_vbox._pack_noexpand_nofill(page)
            page.set_expanded(True)
            page.show_all()
    
    def set_multiple_host_details(self, host_list):
        """Set details for multiple hosts in the list"""

        hosts = []
        for h in host_list:
            hosts.append(h['page'])
        
        return hosts

    def _save_comment(self, widget, extra, host):
        """Sets the comment on a host from the contents of the comment text
        entry."""
        buff = widget.get_buffer()
        host.comment = buff.get_text(buff.get_start_iter(), buff.get_end_iter())
        for scan in self.inventory.get_scans():
            if host in scan.get_hosts():
                scan.unsaved = True
                break
    
    def set_host_details(self, host):
        """Sets up all the host details and updates the host page with this information.
        Connects event to automatically update comments, target, and profile information. Then it
        sets the events to record comments to be saved automatically. It gathers the comments, uptime,
        status variables (incl. state, uptime, and lastboot entries) and attempts to get ipv4, ipv6, mac
        address info and show it. It gets hostname, OS info (if available), as well as TCP/IP info and displays it."""

        # Start connecting event to automatically update comments, target and profile information.
        host_page = ScanHostDetailsPage(host.get_hostname())
        host_details = host_page.host_details

        log.debug(">>> Setting host details")
        log.debug(">>> Hostname: %s" % host.get_hostname())
        log.debug(">>> Comment: %s" % host.comment)
        host_details.set_comment(host.comment)
        

        # Setting events to automatically record the commentary to be maintained
        host_page.host_details.comment_txt_vw.connect("insert-at-cursor", self._save_comment,
                                                      host)
        host_page.host_details.comment_txt_vw.connect("focus-out-event", self._save_comment,
                                                      host)

        
        uptime = host.get_uptime()

        host_details.set_host_status({'state':host.get_state(),
                                      'open':str(host.get_open_ports()),
                                      'filtered':str(host.get_filtered_ports()),
                                      'closed':str(host.get_closed_ports()),
                                      'scanned':str(host.get_scanned_ports()),
                                      'uptime':uptime['seconds'],
                                      'lastboot':uptime['lastboot']})

        addresses = {}
        if host.ip is not None:
            addresses['ipv4'] = host.ip['addr']
        if host.ipv6 is not None:
            addresses['ipv6'] = host.ipv6['addr']
        if host.mac is not None:
            addresses['mac'] = host.mac['addr']
        
        host_details.set_addresses(addresses)
        
        host_details.set_hostnames(host.get_hostnames())
        
        os = host.get_best_osmatch()
        if os:
            os['portsused'] = host.get_ports_used()
            os['osclass'] = host.get_osclasses()
        
        host_details.set_os(os)
        host_details.set_tcpseq(host.get_tcpsequence())
        host_details.set_ipseq(host.get_ipidsequence())
        host_details.set_tcptsseq(host.get_tcptssequence())
        
        return host_page
    
    def set_single_host_port(self, host):
        """For a single host, each of the host's services including 
        hostname, portid, protocol, port_state, service_product, and service_version."""

        host_page = self.scan_result.scan_result_notebook.open_ports.host
        host_page.switch_port_to_list_store()
        
        p = host.get_ports()
        ports = []
        for port in p:
            ports += port['port']
        
        host_page.clear_port_list()
        for p in ports:
            host_page.add_port([self.findout_service_icon(p),
                                int(p.get('portid', '0')),
                                p.get('protocol', ''),
                                p.get('port_state', ''),
                                p.get('service_name', ''),
                                get_version_string(p)])

    def set_single_service_host(self, service):
        """For a single host, add each of the host's services including hostname,
        portid, protocol, port_state, service_product, and service_version."""

        host_page = self.scan_result.scan_result_notebook.open_ports.host
        host_page.switch_host_to_list_store()
        host_page.clear_host_list()

        for h in service:
            host_page.add_host([self.findout_service_icon(h),
                                h.get('hostname', ''),
                                int(h.get('portid', '0')),
                                h.get('protocol', ''),
                                h.get('port_state', ''),
                                get_version_string(h)])
        
    
    def set_multiple_host_port(self, host_list):
        """For multiple hosts in the host list, append a new entry to the tree, and
        get all of its ports, and append them to the respective host tree. Give for each port
                the portid, protocol, port_state, service_name and service_product."""

        host_page = self.scan_result.scan_result_notebook.open_ports.host
        host_page.switch_port_to_tree_store()
        host_page.clear_port_tree()
        
        for host in host_list:
            parent = host_page.port_tree.append(None, 
                [host['host'].get_hostname(), None, 0,'','','',''])
            for port in host['host'].get_ports():
                for p in port.get('port', []):
                    host_page.port_tree.append(parent, \
                                ['',
                                 self.findout_service_icon(p),
                                 int(p.get('portid', "0")),
                                 p.get('protocol', ''),
                                 p.get('port_state', ""),
                                 p.get('service_name', _("Unknown")),
                                 get_version_string(p)])

    def set_multiple_service_host(self, service_list):
        """For multiple hosts that have services in the service list, append its entry to the host tree,
                and for each of these hosts append its properties (hostname, portid, protocol, port_state,
        service_product, service_version)."""

        host_page = self.scan_result.scan_result_notebook.open_ports.host
        host_page.switch_host_to_tree_store()
        host_page.clear_host_tree()
        
        for host in service_list:
            parent = host_page.host_tree.append(None, [host['service_name'],
                                                       '','',0,'','', ''])
            for h in host['hosts']:
                host_page.host_tree.append(parent, \
                                           ['',
                                            self.findout_service_icon(h),
                                            h["hostname"],
                                            int(h.get('portid', "0")),
                                            h.get('protocol', ""),
                                            h.get('port_state', _("Unknown")),
                                            get_version_string(h)])
    
    def findout_service_icon(self, port_info):
        if port_info["port_state"] in ["open", "open|filtered"]:
            return gtk.STOCK_YES
        else:
            return gtk.STOCK_NO

def get_version_string(d):
    """Get a human-readable version string from the dict d. The keys used in d
    are "service_product", "service_version", and "service_extrainfo" (all are
    optional). This produces a string like "OpenSSH 4.3p2 Debian 9etch2
    (protocol 2.0)"."""
    result = []
    if d.get("service_product"):
        result.append(d["service_product"])
    if d.get("service_version"):
        result.append(d["service_version"])
    if d.get("service_extrainfo"):
        result.append("(" + d["service_extrainfo"] + ")")
    return " ".join(result)

class ScanResult(gtk.HPaned):
    """
        ScanResult
        Controls showing the results of a scan.
        Includes controls to change/clear output text, clear the hosts/services, return the
        selected host/services,        obtain and display the nmap output, let the services and hosts
        lists be selected, and refresh output.
        """
    def __init__(self, inventory, scans_store):
        gtk.HPaned.__init__(self)
        
        self.scan_host_view = ScanHostsView()
        self.scan_result_notebook = ScanResultNotebook(inventory, scans_store)

        self.pack1(self.scan_host_view, True, True)
        self.pack2(self.scan_result_notebook, True, False)

    def set_nmap_output(self, msg):
        self.scan_result_notebook.nmap_output.nmap_output.text_view.get_buffer().set_text(msg)

    def clear_nmap_output(self):
        self.scan_result_notebook.nmap_output.nmap_output.text_view.get_buffer().set_text("")

    def clear_host_view(self):
        self.set_hosts({})

    def clear_service_view(self):
        self.set_services({})

    def get_host_selection(self):
        return self.scan_host_view.host_view.get_selection()

    def get_service_selection(self):
        return self.scan_host_view.service_view.get_selection()

    def get_nmap_output(self):
        return self.scan_result_notebook.nmap_output.get_nmap_output()

    def set_hosts(self, hosts_dic):
        """Set hosts to those in host list"""
        self.scan_host_view.set_hosts(hosts_dic)

    def set_services(self, services_dic):
        """Set services to those in services list"""
        self.scan_host_view.set_services(services_dic)

    def clear_port_list(self):
        """Clear the scan result ports list"""
        self.scan_result_notebook.open_ports.host.clear_port_list()

    def change_to_ports_hosts_tab(self):
        """Show the "Ports / Hosts" tab"""
        self.scan_result_notebook.set_current_page(1)

    def change_to_nmap_output_tab(self):
        """Show the nmap output tab"""
        self.scan_result_notebook.set_current_page(0)

    def refresh_nmap_output(self):
        """Refresh the Nmap output with the newest output of command_execution,
        if it is not None."""
        self.scan_result_notebook.nmap_output.nmap_output.refresh_output()
        

class ScanResultNotebook(HIGNotebook):
    """
    ScanResultNotebook
    Creates a new Scan Results notebook page, which includes the sections Ports/Hosts,
    Nmap Output, Host Details, and Scan Details.
    Organizes the way the results for scan is displayed in its new tab.
    """
    def __init__(self, inventory, scans_store):
        HIGNotebook.__init__(self)
        self.set_border_width(5)
        
        self.__create_widgets(inventory, scans_store)
        
        self.scans_list.scans_list.connect("row-activated", self._scan_row_activated)

        self.append_page(self.nmap_output_page, gtk.Label(_('Nmap Output')))
        self.append_page(self.open_ports_page, gtk.Label(_('Ports / Hosts')))
        self.append_page(self.topology_page, gtk.Label(_('Topology')))
        self.append_page(self.host_details_page, gtk.Label(_('Host Details')))
        self.append_page(self.scans_list_page, gtk.Label(_('Scans')))

    def host_mode(self):
        self.open_ports.host.host_mode()

    def port_mode(self):
        self.open_ports.host.port_mode()
    
    def __create_widgets(self, inventory, scans_store):
        self.open_ports_page = HIGVBox()
        self.nmap_output_page = HIGVBox()
        self.topology_page = HIGVBox()
        self.host_details_page = HIGScrolledWindow()
        self.host_details_vbox = HIGVBox()
        self.scans_list_page = HIGVBox()
        
        self.open_ports = ScanOpenPortsPage()
        self.nmap_output = ScanNmapOutputPage(scans_store)
        self.topology = TopologyPage(inventory)
        self.scans_list = ScanScanListPage(scans_store)
        
        self.no_selected = gtk.Label(_('No host selected.'))
        self.host_details = self.no_selected
        
        self.open_ports_page.add(self.open_ports)
        self.nmap_output_page.add(self.nmap_output)
        self.topology_page.add(self.topology)
        self.scans_list_page.add(self.scans_list)
        
        self.host_details_page.add_with_viewport(self.host_details_vbox)
        self.host_details_vbox._pack_expand_fill(self.host_details)
    
    def _scan_row_activated(self, treeview, path, view_column):
        """Switch back to the Nmap Output view when a scan is activated
        (double-clicked) on the scans list."""
        self.nmap_output.set_active_iter(treeview.get_model().get_iter(path))
        self.set_current_page(0)

if __name__ == "__main__":
    pass
