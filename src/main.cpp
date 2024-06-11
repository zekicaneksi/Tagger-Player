#include "main.h"
#include "widgets/mainframe.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    MainFrame *mainFrame = new MainFrame(wxT("My App"));
    mainFrame->Show(true);

    return true;
}
