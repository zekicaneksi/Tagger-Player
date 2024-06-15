#include "TagListBox.h"
#include "../logic/logic.h"
#include <vector>
#include <wx/msgdlg.h>

TagClientData::TagClientData(logic::Tag tag_arg) { tag = tag_arg; }

TagListBox::TagListBox(wxWindow *parent)
    : wxListBox(parent, wxID_ANY, wxPoint(-1, -1), wxSize(-1, -1)) {

  std::vector<logic::Tag> tags = logic::GetTags();
  for (int i = 0; i < tags.size(); i++) {
    this->Append(wxString::FromUTF8(tags[i].name), new TagClientData(tags[i]));
  }
}
