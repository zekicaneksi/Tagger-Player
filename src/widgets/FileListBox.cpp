#include "FileListBox.h"
#include "../logic/logic.h"
#include <vector>
#include <wx/msgdlg.h>

FileClientData::FileClientData(int file_id_arg) { file_id = file_id_arg; }

FileListBox::FileListBox(wxWindow *parent, std::vector<logic::File> *files_arg,
                         wxWindowID windowID)
    : wxListBox(parent, windowID, wxPoint(-1, -1), wxSize(-1, -1)) {

  files = files_arg;

}
