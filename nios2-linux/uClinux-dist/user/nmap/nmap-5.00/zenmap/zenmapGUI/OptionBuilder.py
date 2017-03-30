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

import gobject
import gtk

from xml.dom import minidom

from zenmapGUI.higwidgets.higboxes import HIGHBox
from zenmapGUI.higwidgets.higlabels import HIGEntryLabel
from zenmapGUI.higwidgets.higbuttons import HIGButton

from zenmapGUI.FileChoosers import AllFilesFileChooserDialog
from zenmapGUI.ProfileHelp import ProfileHelp

from zenmapCore.Paths import Path
from zenmapCore.NmapOptions import NmapOptions
import zenmapCore.I18N
from zenmapCore.UmitLogging import log

def get_option_check_auxiliary_widget(option, ops, check):
    if option in ("-sI", "-b", "--script", "--script-args", "--exclude", "-p",
        "-D", "-S", "--source-port", "-e", "--ttl", "-iR", "--max-retries",
        "--host-timeout", "--max-rtt-timeout", "--min-rtt-timeout",
        "--initial-rtt-timeout", "--max-hostgroup", "--min-hostgroup",
        "--max-parallelism", "--min-parallelism", "--max-scan-delay",
        "--scan-delay", "-PA", "-PS", "-PU", "-PO"):
        return OptionEntry(option, ops, check)
    elif option in ("-d", "-v"):
        return OptionLevel(option, ops, check)
    elif option in ("--excludefile", "-iL"):
        return OptionFile(option, ops, check)
    elif option in ("-A", "-O", "-sV", "-n", "-6", "-PN", "-PE", "-PP", "-PM",
        "-PB", "-sC", "--script-trace", "-F", "-f", "--packet-trace", "-r",
        "--traceroute"):
        return None
    elif option in ("",):
        return OptionExtras(option, ops, check)
    else:
        assert False, "Unknown option %s" % option

class OptionEntry(gtk.Entry):
    def __init__(self, option, ops, check):
        gtk.Entry.__init__(self)
        self.option = option
        self.ops = ops
        self.check = check
        self.connect("changed", self.changed_cb)
        self.check.connect("toggled", self.check_toggled_cb)
        self.update()

    def update(self):
        if self.ops[self.option] is not None:
            self.set_text(str(self.ops[self.option]))
            self.check.set_active(True)
        else:
            self.set_text("")
            self.check.set_active(False)

    def check_toggled_cb(self, check):
        if check.get_active():
            self.ops[self.option] = self.get_text().decode("UTF-8")
        else:
            self.ops[self.option] = None

    def changed_cb(self, widget):
        self.check.set_active(True)
        self.ops[self.option] = self.get_text().decode("UTF-8")

class OptionExtras(gtk.Entry):
    def __init__(self, option, ops, check):
        gtk.Entry.__init__(self)
        self.ops = ops
        self.check = check
        self.connect("changed", self.changed_cb)
        self.check.connect("toggled", self.check_toggled_cb)
        self.update()

    def update(self):
        if len(self.ops.extras) > 0:
            self.set_text(" ".join(self.ops.extras))
            self.check.set_active(True)
        else:
            self.set_text("")
            self.check.set_active(False)

    def check_toggled_cb(self, check):
        if check.get_active():
            self.ops.extras = [self.get_text().decode("UTF-8")]
        else:
            self.ops.extras = []

    def changed_cb(self, widget):
        self.check.set_active(True)
        self.ops.extras = [self.get_text().decode("UTF-8")]

class OptionLevel(gtk.SpinButton):
    def __init__(self, option, ops, check):
        gtk.SpinButton.__init__(self, gtk.Adjustment(0, 0, 10, 1), 0.0, 0)
        self.option = option
        self.ops = ops
        self.check = check
        self.connect("changed", self.changed_cb)
        self.check.connect("toggled", self.check_toggled_cb)
        self.update()

    def update(self):
        level = self.ops[self.option]
        if level is not None and level > 0:
            self.get_adjustment().set_value(int(level))
            self.check.set_active(True)
        else:
            self.get_adjustment().set_value(0)
            self.check.set_active(False)

    def check_toggled_cb(self, check):
        if check.get_active():
            self.ops[self.option] = int(self.get_adjustment().get_value())
        else:
            self.ops[self.option] = 0

    def changed_cb(self, widget):
        self.check.set_active(True)
        self.ops[self.option] = int(self.get_adjustment().get_value())

class OptionFile(gtk.HBox):
    __gsignals__ = {
        "changed": (gobject.SIGNAL_RUN_FIRST, gobject.TYPE_NONE, ())
    }

    def __init__(self, option, ops, check):
        gtk.HBox.__init__(self)

        self.option = option
        self.ops = ops
        self.check = check

        self.entry = gtk.Entry()
        self.pack_start(self.entry, True, True)
        button = HIGButton(stock = gtk.STOCK_OPEN)
        self.pack_start(button, False)

        button.connect("clicked", self.clicked_cb)

        self.entry.connect("changed", lambda x: self.emit("changed"))
        self.entry.connect("changed", self.changed_cb)
        self.check.connect("toggled", self.check_toggled_cb)
        self.update()

    def update(self):
        if self.ops[self.option] is not None:
            self.entry.set_text(self.ops[self.option])
            self.check.set_active(True)
        else:
            self.entry.set_text("")
            self.check.set_active(False)

    def check_toggled_cb(self, check):
        if check.get_active():
            self.ops[self.option] = self.entry.get_text().decode("UTF-8")
        else:
            self.ops[self.option] = None

    def changed_cb(self, widget):
        self.check.set_active(True)
        self.ops[self.option] = self.entry.get_text().decode("UTF-8")

    def clicked_cb(self, button):
        dialog = AllFilesFileChooserDialog(_("Choose file"))
        if dialog.run() == gtk.RESPONSE_OK:
            self.entry.set_text(dialog.get_filename())
        dialog.destroy()

class TargetEntry(gtk.Entry):
    def __init__(self, ops):
        gtk.Entry.__init__(self)
        self.ops = ops
        self.connect("changed", self.changed_cb)
        self.update()

    def update(self):
        self.set_text(u" ".join(self.ops.target_specs))

    def changed_cb(self, eidget):
        self.ops.target_specs = self.get_targets()

    def get_targets(self):
        return self.get_text().decode("UTF-8").split()

class OptionTab(object):
    def __init__(self, root_tab, ops, update_command, update_help):
        actions = {'target':self.__parse_target,
                   'option_list':self.__parse_option_list,
                   'option_check':self.__parse_option_check}

        self.ops = ops
        self.update_command = update_command
        self.update_help = update_help

        self.profilehelp = ProfileHelp()

        self.widgets_list = []
        for option_element in root_tab.childNodes:
            try:option_element.tagName
            except:pass
            else:
                if option_element.tagName in actions.keys():
                    parse_func = actions[option_element.tagName]
                    widget = parse_func(option_element)
                    self.widgets_list.append(widget)

    def __parse_target(self, target_element):
        label = target_element.getAttribute(u'label')
        label_widget = HIGEntryLabel(label)
        target_widget = TargetEntry(self.ops)
        target_widget.connect("changed", self.update_target)
        return label_widget, target_widget

    def __parse_option_list(self, option_list_element):
        children = option_list_element.getElementsByTagName(u'option')
        
        label_widget = HIGEntryLabel(option_list_element.getAttribute(u'label'))
        option_list_widget = OptionList(self.ops)
        
        for child in children:
            option = child.getAttribute(u'option')
            argument = child.getAttribute(u'argument')
            label = child.getAttribute(u'label')
            option_list_widget.append(option, argument, label)
            self.profilehelp.add_label(option, label)
            self.profilehelp.add_shortdesc(option, child.getAttribute(u'short_desc'))
            self.profilehelp.add_example(option, child.getAttribute(u'example'))

        option_list_widget.update()

        option_list_widget.connect("changed", self.update_list_option)
                
        return label_widget, option_list_widget
    
    def __parse_option_check(self, option_check):
        arg_type = option_check.getAttribute(u'arg_type')
        option = option_check.getAttribute(u'option')
        label = option_check.getAttribute(u'label')
        short_desc = option_check.getAttribute(u'short_desc')
        example = option_check.getAttribute(u'example')

        self.profilehelp.add_label(option, label)
        self.profilehelp.add_shortdesc(option, short_desc)
        self.profilehelp.add_example(option, example)
        
        check = OptionCheck(option, label)
        auxiliary_widget = get_option_check_auxiliary_widget(option, self.ops, check)
        if auxiliary_widget is not None:
            auxiliary_widget.connect("changed", self.update_auxiliary_widget)
            auxiliary_widget.connect('enter-notify-event', self.enter_notify_event_cb, option)
        else:
            check.set_active(not not self.ops[option])

        check.connect('toggled', self.update_check, auxiliary_widget)
        check.connect('enter-notify-event', self.enter_notify_event_cb, option)
        
        return check, auxiliary_widget
        
    def fill_table(self, table, expand_fill = True):
        yopt = (0, gtk.EXPAND | gtk.FILL)[expand_fill]
        for y, widget in enumerate(self.widgets_list):
            if widget[1] == None:
                table.attach(widget[0], 0, 2, y, y+1, yoptions=yopt)
            else:
                table.attach(widget[0], 0, 1, y, y+1, yoptions=yopt)
                table.attach(widget[1], 1, 2, y, y+1, yoptions=yopt)

    def update_auxiliary_widget(self, auxiliary_widget):
        self.update_command()

    def update(self):
        for check, auxiliary_widget in self.widgets_list:
            if auxiliary_widget is not None:
                auxiliary_widget.update()
            else:
                check.set_active(not not self.ops[check.option])

    def update_target(self, entry):
        self.ops.target_specs = entry.get_targets()
        self.update_command()

    def update_check(self, check, auxiliary_widget):
        if auxiliary_widget is None:
            if check.get_active():
                self.ops[check.option] = True
            else:
                self.ops[check.option] = False
        self.update_command()

    def update_list_option(self, widget):
        if widget.last_selected:
            self.ops[widget.last_selected] = None
        
        opt, arg, label = widget.list[widget.get_active()]
        if opt:
            if arg:
                self.ops[opt] = arg
            else:
                self.ops[opt] = True

        widget.last_selected = opt

        self.show_help_for_option(opt)

        self.update_command()
        
    def show_help_for_option(self, option):
        self.profilehelp.handler(option)
        text = ""
        if self.profilehelp.get_currentstate() == "Default":
            text = ""
        else:
            text += self.profilehelp.get_label()
            text += "\n\n"
            text += self.profilehelp.get_shortdesc()
            if self.profilehelp.get_example():
                text += "\n\nExample input:\n"
                text += self.profilehelp.get_example()
        self.update_help(text)

    def enter_notify_event_cb(self, event, widget, option):
        self.show_help_for_option(option)

class OptionBuilder(object):
    def __init__(self, xml_file, ops, update_func, update_help):
        """
        xml_file is a UI description xml-file
        ops is an NmapOptions instance
        """
        xml_desc = open(xml_file)
        self.xml = minidom.parse(xml_desc)
        # Closing file to avoid problems with file descriptors
        xml_desc.close()

        self.ops = ops
        self.update_help = update_help 
        self.update_func = update_func
        
        self.root_tag = "interface"
        
        self.xml = self.xml.getElementsByTagName(self.root_tag)[0]
        
        self.groups = self.__parse_groups()
        self.section_names = self.__parse_section_names()
        self.tabs = self.__parse_tabs()
    
    def update(self):
        for tab in self.tabs.values():
            tab.update()

    def __parse_section_names(self):
        dic = {}
        for group in self.groups:
            grp = self.xml.getElementsByTagName(group)[0]
            dic[group] = grp.getAttribute(u'label')
        return dic
    
    def __parse_groups(self):
        return [g_name.getAttribute(u'name') for g_name in \
                  self.xml.getElementsByTagName(u'groups')[0].\
                  getElementsByTagName(u'group')]

    def __parse_tabs(self):
        dic = {}
        for tab_name in self.groups:
            dic[tab_name] = OptionTab(self.xml.getElementsByTagName(tab_name)[0],
                                      self.ops, self.update_func, self.update_help)
        return dic


class OptionList(gtk.ComboBox):
    def __init__(self, ops):
        self.ops = ops

        self.list = gtk.ListStore(str, str, str)
        gtk.ComboBox.__init__(self, self.list)

        cell = gtk.CellRendererText()
        self.pack_start(cell, True)
        self.add_attribute(cell, 'text', 2)
        
        self.last_selected = None
        self.options = []
    
    def update(self):
        selected = 0
        for i, row in enumerate(self.list):
            opt, arg = row[0], row[1]
            if opt == "":
                continue
            if (not arg and self.ops[opt]) or (arg and str(self.ops[opt]) == arg):
                selected = i
        self.set_active(selected)

    def append(self, option, argument, label):
        opt = label
        ops = NmapOptions()
        if option is not None and option != "":
            if argument:
                ops[option] = argument
            else:
                ops[option] = True
            opt += " (%s)" % ops.render_string()

        self.list.append([option, argument, opt])
        self.options.append(option)

class OptionCheck(gtk.CheckButton):
    def __init__(self, option, label):
        opt = label
        if option is not None and option != "":
            opt += " (%s)" % option

        gtk.CheckButton.__init__(self, opt, use_underline=False)
        
        self.option = option
