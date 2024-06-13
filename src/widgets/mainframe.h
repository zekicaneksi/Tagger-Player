#include <wx/listbox.h>
#include <wx/wx.h>

class MainFrame : public wxFrame {
public:
  MainFrame(const wxString &title);

  wxListBox *listbox;
};

const int ID_LISTBOX = 5;
