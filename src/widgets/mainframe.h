#ifndef MAINFRAME_H
#define MAINFRAME_H

#include "FileListBox.h"
#include "TagCheckListBox.h"
#include <wx/wx.h>

class MainFrame : public wxFrame {
public:
  MainFrame(const wxString &title, wxSize minAndInitialSize);

  std::vector<logic::File> files;
  std::vector<logic::Tag> tags;

  std::vector<int> filteredTagIds;

  FileListBox *fileListBox;
  TagCheckListBox *tagCheckListBox;
  wxListBox *attachedTagsListBox;
  wxListBox *unattachedTagsListBox;

  wxTextCtrl *fileFilterCtrl;

  wxButton *createTagBtn;
  wxButton *deleteTagBtn;
  wxButton *renameTagBtn;
  wxButton *attachTagBtn;
  wxButton *detachTagBtn;

  void FileFilterOnTextChange(wxCommandEvent &event);
  void ListBoxChange_State_DetachBtn(wxCommandEvent &event);
  void ListBoxChange_State_AttachBtn(wxCommandEvent &event);
  void FileListBoxChange(wxCommandEvent &event);
  void TagCheckListBoxChange(wxCommandEvent &event);
  void TagFilter(wxCommandEvent &event);

  void CreateTagBtn(wxCommandEvent &event);
  void DeleteTagBtn(wxCommandEvent &event);
  void RenameTagBtn(wxCommandEvent &event);
  void AttachTagBtn(wxCommandEvent &event);
  void DetachTagBtn(wxCommandEvent &event);
};

// Window Ids
const int CREATE_TAG_BTN = 1;
const int ATTACH_TAG_BTN = 2;
const int FILE_FILTER_CTRL = 3;
const int FILE_LISTBOX = 4;
const int TAG_LISTCHECKBOX = 5;
const int ATTACHED_TAGS_LISTBOX = 6;
const int UNATTACHED_TAGS_LISTBOX = 7;
const int DETACH_TAG_BTN = 8;
const int DELETE_TAG_BTN = 9;
const int RENAME_TAG_BTN = 10;

#endif
