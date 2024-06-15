#ifndef TAGLISTBOX_H
#define TAGLISTBOX_H

#include "../logic/logic.h"
#include <wx/listbox.h>

class TagListBox : public wxListBox {
public:
  TagListBox(wxWindow *parent);
};

class TagClientData : public wxClientData {
public:
  TagClientData(logic::Tag tag_arg);

  logic::Tag tag;
};

#endif
