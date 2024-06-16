#include "FileListBox.h"
#include "../logic/logic.h"
#include <vector>
#include <wx/msgdlg.h>

FileClientData::FileClientData(logic::File file_arg) { file = file_arg; }

void FileListBox::OnDblClick(wxCommandEvent &event) {
  int sel = this->GetSelection();
  if (sel != -1) {
    FileClientData *myFile =
        static_cast<FileClientData *>(this->GetClientObject(sel));
    wxString Foobar;
    Foobar.Printf(wxT("database id: %d, name: %s"), myFile->file.id,
                  wxString::FromUTF8(myFile->file.name));
    wxMessageBox(Foobar);
  }
}

FileListBox::FileListBox(wxWindow *parent, std::vector<logic::File> *files)
    : wxListBox(parent, wxID_ANY, wxPoint(-1, -1), wxSize(-1, -1)) {

  for (int i = 0; i < files->size(); i++) {
    this->Append(wxString::FromUTF8((*files)[i].name),
                 new FileClientData((*files)[i]));
  }

  Connect(wxEVT_COMMAND_LISTBOX_DOUBLECLICKED,
          wxCommandEventHandler(FileListBox::OnDblClick));
}
