Interactive Qt graphViz display
===============================

Features :
----------

* Use cgraph lib (C++ API wrapper)
* Support edges : label on edge, simple arrow both direction
* Support Nodes : box or ellipse shape, image on Node
* Support subGraphs
* Interactions with node and edge (context menu and double click)
* Zoom/move on graph
* Tested only on dot engine
* Only 6 headers/sources files to add

Screen capture :
----------------

![Screen capture](http://i39.tinypic.com/2gy1z0h.png)


Installation :
--------------

* Download qgv sources from GIT : git clone https://code.google.com/p/qgv/
* Download graphViz librairie : http://www.graphviz.org/Download.php
* Configure GRAPHVIZ_PATH in QGraphViz.pro
* Open with Qt Creator & compile

TODO :
------

* Support more attributes of Graphviz
* Add more comments
* Support head/tail label on edge
* Fully support layout from dot language (may be works...)
* Dynamicaly move node and redraw layout ? (I dont know if it's possible)

Good lecture on subject :
-------------------------

* Steve Dodier-Lazaro : <http://www.mupuf.org/blog/2010/07/08/how_to_use_graphviz_to_draw_graphs_in_a_qt_graphics_scene/>
* Arvin Schnell : <http://arvin.schnell-web.net/qgraph/>
