#include "TagListBox.h"
#include "../logic/logic.h"
#include <vector>
#include <wx/msgdlg.h>

TagClientData::TagClientData(int tag_id_arg) { tag_id = tag_id_arg; }

TagListBox::TagListBox(wxWindow *parent, std::vector<logic::Tag> *tags_arg, wxWindowID windowID)
    : wxListBox(parent, windowID, wxPoint(-1, -1), wxSize(-1, -1)) {

  tags = tags_arg;

  const int tagsSize = tags->size();
  for (int i = 0; i < tagsSize; i++) {
    this->Append(wxString::FromUTF8((*tags)[i].name),
                 new TagClientData((*tags)[i].id));
  }
}
