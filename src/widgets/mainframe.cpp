#include "mainframe.h"
#include "../logic/logic.h"
#include <iostream>
#include <string>
#include <vector>

MainFrame::MainFrame(const wxString &title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(350, 500)) {

  wxPanel *panel = new wxPanel(this, -1);
  wxBoxSizer *hbox = new wxBoxSizer(wxVERTICAL);

  wxStaticText *infoText = new wxStaticText(
      panel, wxID_ANY,
      "double click an element in the listbox to see its info");
  fileListBox = new FileListBox(panel);
  wxStaticText *tagsText = new wxStaticText(panel, wxID_ANY, "Tags");
  tagListBox = new TagListBox(panel);
  createTagBtn = new wxButton(panel, CREATE_TAG_BTN, wxT("Create Tag"));

  hbox->Add(infoText, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 10);
  hbox->Add(fileListBox, 3, wxEXPAND | wxALL, 20);
  hbox->Add(tagsText, 0, wxALIGN_CENTER_HORIZONTAL);
  hbox->Add(tagListBox, 3, wxEXPAND | wxALL, 20);
  hbox->Add(createTagBtn, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM, 10);

  Connect(CREATE_TAG_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          wxCommandEventHandler(MainFrame::CreateTagBtn));

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
