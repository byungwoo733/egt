@page api_changes API Changes

[TOC]

Here, you can find the API changes from the version 1.0. In addition, some hints
are provided to help you update your code.

@section v1_0_1 1.0.1

@subsection combobox ComboBox

The changes about ComboBox are about providing a common interface with ListBox.

@li Rename remove() to remove_item().
@li Uses StringItem as item type instead of string.
@code{.unparsed}
- combo->add_item("Item1");
+ combo->add_item(std::make_shared<egt::StringItem>("Item1"))
@endcode
@code{.unparsed}
auto s = combo->item_at(i);
- if (s == "ok")
+ if (s->text() == "ok")
@endcode
@li Add show() and hide() member functions.

@subsection listbox ListBox

@li Restrict items to StringItem instead of Widgets.

@subsection staticgrid StaticGrid

@li Add n_col() and n_row() member functions to get the number of columns and
rows.
@li Add add() member functions taking a GridPoint as parameter instead of column
and row. Prefer these new member functions to the older ones.
@code{.unparsed}
- grid.add(button, 2, 1);
+ grid.add(button, egt::StaticGrid::GridPoint(2, 1));
@endcode
