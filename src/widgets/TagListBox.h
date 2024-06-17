#ifndef TAGLISTBOX_H
#define TAGLISTBOX_H

#include "../logic/logic.h"
#include <wx/listbox.h>

class TagListBox : public wxListBox {
public:
  std::vector<logic::Tag> *tags;

  TagListBox(wxWindow *parent, std::vector<logic::Tag> *tags_arg,
             wxWindowID windowID);
};

class TagClientData : public wxClientData {
public:
  TagClientData(int tag_id_arg);

  int tag_id;
};

#endif
