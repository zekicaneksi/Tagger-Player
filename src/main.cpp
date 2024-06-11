#include "main.h"
#include "widgets/mainframe.h"

IMPLEMENT_APP(App)

bool App::OnInit()
{
    MainFrame *mainFrame = new MainFrame(wxT("Tagger"));
    mainFrame->Show(true);

    return true;
}
