#include "main.h"
#include "widgets/mainframe.h"
#include "logic/logic.h"

IMPLEMENT_APP(App)

bool App::OnInit()
{
    // Get Directory Path
    wxDirDialog dirDialog(NULL, "Choose input directory", "", wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

        if (dirDialog.ShowModal() == wxID_CANCEL)
        return false;

        logic::dirPath = dirDialog.GetPath().ToStdWstring();

    MainFrame *mainFrame = new MainFrame(wxT("Tagger"));
    mainFrame->Show(true);

    return true;
}
