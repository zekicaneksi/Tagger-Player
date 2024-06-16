#ifndef MAINFRAME_H
#define MAINFRAME_H

#include "FileListBox.h"
#include "TagListBox.h"
#include <wx/wx.h>

class MainFrame : public wxFrame {
public:
  MainFrame(const wxString &title);

  std::vector<logic::File> files;

  FileListBox *fileListBox;
  TagListBox *tagListBox;

  wxButton *createTagBtn;
  wxTextCtrl *fileFilterCtrl;

  void CreateTagBtn(wxCommandEvent &event);
  void FileFilterOnTextChange(wxCommandEvent &event);
};

// Window Ids
const int CREATE_TAG_BTN = 1;
const int FILE_FILTER_CTRL = 2;

#endif
