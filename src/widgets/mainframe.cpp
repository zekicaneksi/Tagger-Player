#include "mainframe.h"
#include <wx/textdlg.h>


MainFrame::MainFrame(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(270, 200))
{

  wxPanel * panel = new wxPanel(this, -1);

  wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

  listbox = new wxListBox(panel, ID_LISTBOX, 
      wxPoint(-1, -1), wxSize(-1, -1)); 

  hbox->Add(listbox, 3, wxEXPAND | wxALL, 20);

  panel->SetSizer(hbox);
  Center();  
}
