#include "main.h"
#include "logic/logic.h"
#include "widgets/mainframe.h"

IMPLEMENT_APP(App)

bool App::OnInit() {
  // Get Directory Path
  wxDirDialog dirDialog(NULL, "Choose input directory", "",
                        wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

  if (dirDialog.ShowModal() == wxID_CANCEL)
    return false;

  logic::dirPath = dirDialog.GetPath().utf8_str();

  if (logic::initDB() != 0) {
    wxMessageBox(wxT("Error initializing db, check console output"));
    return false;
  }
  if (logic::updateFiles() != 0) {
    wxMessageBox(wxT("Error updating the files, check console output"));
    return false;
  }

  MainFrame *mainFrame = new MainFrame(wxT("Tagger Player"), wxSize(750, 750));
  mainFrame->Show(true);

  return true;
}
