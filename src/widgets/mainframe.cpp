#include "mainframe.h"
#include "../logic/logic.h"
#include "Utility.h"
#include <iostream>
#include <string>
#include <vector>

MainFrame::MainFrame(const wxString &title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(350, 500)) {

  files = logic::GetFiles();

  wxPanel *panel = new wxPanel(this, -1);
  wxBoxSizer *hbox = new wxBoxSizer(wxVERTICAL);

  wxStaticText *infoText = new wxStaticText(
      panel, wxID_ANY,
      "double click an element in the listbox to see its info");
  fileFilterCtrl = new wxTextCtrl(panel, FILE_FILTER_CTRL);
  fileListBox = new FileListBox(panel, &files);
  wxStaticText *tagsText = new wxStaticText(panel, wxID_ANY, "Tags");
  tagListBox = new TagListBox(panel);
  createTagBtn = new wxButton(panel, CREATE_TAG_BTN, wxT("Create Tag"));

  hbox->Add(infoText, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 10);
  hbox->Add(fileFilterCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 10);
  hbox->Add(fileListBox, 3, wxEXPAND | wxALL, 20);
  hbox->Add(tagsText, 0, wxALIGN_CENTER_HORIZONTAL);
  hbox->Add(tagListBox, 3, wxEXPAND | wxALL, 20);
  hbox->Add(createTagBtn, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM, 10);

  Connect(CREATE_TAG_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          wxCommandEventHandler(MainFrame::CreateTagBtn));
  Connect(FILE_FILTER_CTRL, wxEVT_TEXT,
          wxCommandEventHandler(MainFrame::FileFilterOnTextChange));

  panel->SetSizer(hbox);
  Center();
}

void MainFrame::CreateTagBtn(wxCommandEvent &event) {
  wxString str = wxGetTextFromUser(wxT("Tag Name"), wxT("Add Tag"));

  if (str.Len() == 0)
    return;

  if (logic::createTag(std::string(str.utf8_str())) != 0) {
    wxMessageBox(
        wxT("Failed to insert into database, check console! Quitting..."));
    Close(true);
  }
  tagListBox->Append(str);
}

void MainFrame::FileFilterOnTextChange(wxCommandEvent &event) {
  std::string inputText = std::string(event.GetString().utf8_str());
  fileListBox->Clear();
  int fileCount = files.size();
  std::vector<std::pair<int, int>> filterMap(fileCount);

  for (int i = 0; i < fileCount; i++) {
    filterMap[i] =
        std::make_pair(i, levenshteinDistance(inputText, files[i].name));
  }

  std::sort(filterMap.begin(), filterMap.end(),
            [](std::pair<int, int> &left, std::pair<int, int> &right) {
              return left.second < right.second;
            });

  for (int i = 0; i < filterMap.size(); i++) {
    fileListBox->Append(wxString::FromUTF8(files[filterMap[i].first].name),
                        new FileClientData(files[filterMap[i].first]));
  }
}
