#ifndef MAINFRAME_H
#define MAINFRAME_H

#include "FileListBox.h"
#include <wx/wx.h>

class MainFrame : public wxFrame {
public:
  MainFrame(const wxString &title);

  FileListBox *listBox;
};

#endif
