#include "TagCheckListBox.h"
#include "../logic/logic.h"
#include <vector>
#include <wx/msgdlg.h>

TagClientData::TagClientData(int tag_id_arg) { tag_id = tag_id_arg; }

TagCheckListBox::TagCheckListBox(wxWindow *parent, std::vector<logic::Tag> *tags_arg, wxWindowID windowID)
    : wxCheckListBox(parent, windowID, wxPoint(-1, -1), wxSize(-1, -1)) {

  tags = tags_arg;

  const int tagsSize = tags->size();
  for (int i = 0; i < tagsSize; i++) {
    this->Append(wxString::FromUTF8((*tags)[i].name),
                 new TagClientData((*tags)[i].id));
  }
}
