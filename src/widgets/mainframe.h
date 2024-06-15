#ifndef MAINFRAME_H
#define MAINFRAME_H

#include "FileListBox.h"
#include "TagListBox.h"
#include <wx/wx.h>

class MainFrame : public wxFrame {
public:
  MainFrame(const wxString &title);

  FileListBox *fileListBox;
  TagListBox *tagListBox;

  wxButton *createTagBtn;

  void CreateTagBtn(wxCommandEvent &event);
};

// Window Ids
const int CREATE_TAG_BTN = 1;

#endif
