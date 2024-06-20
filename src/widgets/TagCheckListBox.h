#ifndef TAGCHECKLISTBOX_H
#define TAGCHECKLISTBOX_H

#include "../logic/logic.h"
#include <wx/checklst.h>

class TagCheckListBox : public wxCheckListBox {
public:
  std::vector<logic::Tag> *tags;

  TagCheckListBox(wxWindow *parent, std::vector<logic::Tag> *tags_arg,
             wxWindowID windowID);
};

class TagClientData : public wxClientData {
public:
  TagClientData(int tag_id_arg);

  int tag_id;
};

#endif
