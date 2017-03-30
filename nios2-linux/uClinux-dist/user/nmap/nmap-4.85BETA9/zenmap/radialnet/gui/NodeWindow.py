# vim: set fileencoding=utf-8 :

# Copyright (C) 2007, 2008 Insecure.Com LLC.
#
# Author: João Paulo de Souza Medeiros <ignotus21@gmail.com>
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

import gtk
import pango

import radialnet.util.drawing as drawing

from radialnet.bestwidgets.windows import *
from radialnet.bestwidgets.boxes import *
from radialnet.bestwidgets.labels import *
from radialnet.gui.Image import Application
from radialnet.gui.NodeNotebook import NodeNotebook


DIMENSION_NORMAL = (600, 400)


class NodeWindow(BWWindow):
    """
    """
    def __init__(self, node, position):
        """
        """
        BWWindow.__init__(self, gtk.WINDOW_TOPLEVEL)
        self.move(position[0], position[1])
        self.set_default_size(DIMENSION_NORMAL[0], DIMENSION_NORMAL[1])

        self.__node = node

        self.__title_font = pango.FontDescription('Monospace Bold')

        self.__icon = Application()
        self.__create_widgets()


    def __create_widgets(self):
        """
        """
        self.__content = BWVBox()
        self.__head = BWHBox(spacing=2)

        self.__notebook = NodeNotebook(self.__node)

        # create head elements

        # icon with node's score color
        self.__color_box = gtk.EventBox()
        self.__color_image = gtk.Image()
        self.__color_image.set_from_file(self.__icon.get_icon('border'))
        self.__color_box.add(self.__color_image)
        self.__color_box.set_size_request(15, 15)
        r, g, b = drawing.cairo_to_gdk_color(self.__node.get_draw_info('color'))
        self.__color_box.modify_bg(gtk.STATE_NORMAL, gtk.gdk.Color(r, g, b))

        # title with the node ip and hostname
        self.__title = ""

        if self.__node.get_info('hostname') != None:
            self.__title = self.__node.get_info('hostname') + ' - '

        self.__title += self.__node.get_info('ip')

        self.set_title(self.__title)

        self.__title_label = BWSectionLabel(self.__title)
        self.__title_label.modify_font(self.__title_font)

        # packing head elements
        self.__head.bw_pack_start_noexpand_nofill(self.__color_box)
        self.__head.bw_pack_start_expand_fill(self.__title_label)

        # packing all to content
        self.__content.bw_pack_start_noexpand_nofill(self.__head)
        self.__content.bw_pack_start_expand_fill(self.__notebook)

        # add content to window
        self.add(self.__content)
