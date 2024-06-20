#include "mainframe.h"
#include "../logic/logic.h"
#include "Utility.h"
#include <iostream>
#include <string>
#include <vector>

MainFrame::MainFrame(const wxString &title, wxSize minAndInitialSize)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, minAndInitialSize) {

  files = logic::GetFiles();
  tags = logic::GetTags();

  wxPanel *panel = new wxPanel(this, wxID_ANY);

  // Creating the widgets
  wxStaticText *tagsText = new wxStaticText(panel, wxID_ANY, "Tags");
  wxStaticText *filesText = new wxStaticText(panel, wxID_ANY, "Files");
  wxStaticText *placeholderText =
      new wxStaticText(panel, wxID_ANY, "Place holder text");
  wxStaticText *attachedTagsText =
      new wxStaticText(panel, wxID_ANY, "Attached Tags");
  wxStaticText *unattachedTagsText =
      new wxStaticText(panel, wxID_ANY, "Unattached Tags");

  fileListBox = new FileListBox(panel, &files, FILE_LISTBOX);
  tagCheckListBox = new TagCheckListBox(panel, &tags, TAG_LISTBOX);
  attachedTagsListBox = new wxListBox(panel, ATTACHED_TAGS_LISTBOX);
  attachedTagsListBox->Disable();
  unattachedTagsListBox = new wxListBox(panel, UNATTACHED_TAGS_LISTBOX);
  unattachedTagsListBox->Disable();

  fileFilterCtrl = new wxTextCtrl(panel, FILE_FILTER_CTRL);

  createTagBtn = new wxButton(panel, CREATE_TAG_BTN, wxT("Create Tag"));
  attachTagBtn = new wxButton(panel, ATTACH_TAG_BTN, wxT("Attach Tag"));
  attachTagBtn->Disable();
  detachTagBtn = new wxButton(panel, DETACH_TAG_BTN, wxT("Detach Tag"));
  detachTagBtn->Disable();

  // Creating the layout
  wxBoxSizer *mainBox = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer *leftSideBox = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *rightSideBox = new wxBoxSizer(wxVERTICAL);
  wxPanel *separatorPanel = new wxPanel(panel, wxID_ANY, wxDefaultPosition);

  mainBox->Add(leftSideBox, 1, wxEXPAND | wxBOTTOM | wxRIGHT | wxLEFT, 20);
  mainBox->Add(separatorPanel, 0, wxEXPAND | wxTOP | wxBOTTOM, 20);
  mainBox->Add(rightSideBox, 1, wxEXPAND | wxBOTTOM | wxRIGHT | wxLEFT, 20);

  // Left side layout
  wxBoxSizer *tagAttachBox = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer *attachedTagBox = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *unattachedTagBox = new wxBoxSizer(wxVERTICAL);

  leftSideBox->Add(tagsText, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 10);
  leftSideBox->Add(tagCheckListBox, 1, wxEXPAND | wxTOP | wxBOTTOM, 10);
  leftSideBox->Add(createTagBtn, 0, wxEXPAND | wxBOTTOM, 10);
  leftSideBox->Add(filesText, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 10);
  leftSideBox->Add(fileFilterCtrl, 0, wxEXPAND | wxTOP, 10);
  leftSideBox->Add(fileListBox, 1, wxEXPAND | wxBOTTOM | wxTOP, 10);

  leftSideBox->Add(tagAttachBox, 1, wxEXPAND);
  tagAttachBox->Add(attachedTagBox, 1, wxEXPAND);
  tagAttachBox->Add(unattachedTagBox, 1, wxEXPAND);

  attachedTagBox->Add(attachedTagsText, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM,
                      10);
  attachedTagBox->Add(attachedTagsListBox, 1, wxEXPAND | wxRIGHT, 10);
  attachedTagBox->Add(detachTagBtn, 0, wxEXPAND | wxTOP | wxRIGHT, 10);

  unattachedTagBox->Add(unattachedTagsText, 0,
                        wxALIGN_CENTER_HORIZONTAL | wxBOTTOM, 10);
  unattachedTagBox->Add(unattachedTagsListBox, 1, wxEXPAND | wxLEFT, 10);
  unattachedTagBox->Add(attachTagBtn, 0, wxEXPAND | wxTOP | wxLEFT, 10);

  // Right side layout
  rightSideBox->Add(placeholderText, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 10);

  // Styling
  wxColourDatabase colorDB;
  separatorPanel->SetBackgroundColour(colorDB.Find(wxT("DARK GREY")));

  // Events
  Connect(CREATE_TAG_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          wxCommandEventHandler(MainFrame::CreateTagBtn));
  Connect(ATTACH_TAG_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          wxCommandEventHandler(MainFrame::AttachTagBtn));
  Connect(DETACH_TAG_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          wxCommandEventHandler(MainFrame::DetachTagBtn));
  Connect(FILE_FILTER_CTRL, wxEVT_TEXT,
          wxCommandEventHandler(MainFrame::FileFilterOnTextChange));
  Connect(FILE_LISTBOX, wxEVT_LISTBOX,
          wxCommandEventHandler(MainFrame::ListBoxChange_State_AttachBtn));
  Connect(FILE_LISTBOX, wxEVT_LISTBOX,
          wxCommandEventHandler(MainFrame::ListBoxChange_State_DetachBtn));
  Connect(UNATTACHED_TAGS_LISTBOX, wxEVT_LISTBOX,
          wxCommandEventHandler(MainFrame::ListBoxChange_State_AttachBtn));
  Connect(ATTACHED_TAGS_LISTBOX, wxEVT_LISTBOX,
          wxCommandEventHandler(MainFrame::ListBoxChange_State_DetachBtn));
  Connect(FILE_LISTBOX, wxEVT_LISTBOX,
          wxCommandEventHandler(MainFrame::FileListBoxChange));

  // Let's go
  panel->SetSizer(mainBox);
  Center();
  SetMinSize(minAndInitialSize);
}

void MainFrame::CreateTagBtn(wxCommandEvent &event) {
  wxString newTagName = wxGetTextFromUser(wxT("Tag Name"), wxT("Add Tag"));
  std::string newTagNameUtf8 = std::string(newTagName.utf8_str());

  if (newTagName.Len() == 0)
    return;

  // Checking if a tag with same name exists
  const int tagsSize = tags.size();
  for (int i = 0; i < tagsSize; i++) {
    if (tags[i].name == newTagNameUtf8) {
      wxString Msg;
      Msg.Printf(wxT("Tag with the name: %s\nalready exists."), newTagName);
      wxMessageBox(Msg);
      return;
    }
  }

  // Creating Tag
  const int insertedId = logic::createTag(newTagNameUtf8);
  if (insertedId == -1) {
    wxMessageBox(
        wxT("Failed to insert into database, check console! Quitting..."));
    Close(true);
  }

  // Updating
  tags.push_back(logic::Tag{insertedId, newTagNameUtf8});
  tagCheckListBox->Append(newTagName, new TagClientData(insertedId));
  if (fileListBox->GetSelection() != wxNOT_FOUND) {
    unattachedTagsListBox->Append(newTagName, new TagClientData{insertedId});
  }
}

void MainFrame::AttachTagBtn(wxCommandEvent &event) {
  int fileSel = fileListBox->GetSelection();
  int tagSel = unattachedTagsListBox->GetSelection();

  FileClientData *selectedFile =
      static_cast<FileClientData *>(fileListBox->GetClientObject(fileSel));
  TagClientData *selectedTag = static_cast<TagClientData *>(
      unattachedTagsListBox->GetClientObject(tagSel));

  // Loop "files" and find the file
  const int filesSize = files.size();
  for (int i = 0; i < filesSize; i++) {
    if (files[i].id != selectedFile->file_id)
      continue;

    // File found

    // Attach the tag
    if (logic::attachTag(files[i].id, selectedTag->tag_id) != 0) {
      wxMessageBox("Inserting failed, check console. Closing...");
      Close();
    }

    // Attaching the tag is successful
    // Update
    files[i].tag_ids.push_back(selectedTag->tag_id);

    const int tagsSize = tags.size();
    for (int y = 0; y < tagsSize; y++) {
      if (tags[y].id == selectedTag->tag_id) {
        attachedTagsListBox->Append(wxString::FromUTF8(tags[y].name),
                                    new TagClientData(selectedTag->tag_id));
        unattachedTagsListBox->Delete(tagSel);
        unattachedTagsListBox->SetSelection(wxNOT_FOUND);
        attachTagBtn->Disable();
        break;
      }
    }

    break;
  }
}

void MainFrame::DetachTagBtn(wxCommandEvent &event) {
  int fileSel = fileListBox->GetSelection();
  int tagSel = attachedTagsListBox->GetSelection();

  FileClientData *selectedFile =
      static_cast<FileClientData *>(fileListBox->GetClientObject(fileSel));
  TagClientData *selectedTag = static_cast<TagClientData *>(
      attachedTagsListBox->GetClientObject(tagSel));

  // Loop "files" and find the file
  const int filesSize = files.size();
  for (int i = 0; i < filesSize; i++) {
    if (files[i].id != selectedFile->file_id)
      continue;

    // File found

    // Detach the tag
    if (logic::detachTag(files[i].id, selectedTag->tag_id) != 0) {
      wxMessageBox("Inserting failed, check console. Closing...");
      Close();
    }

    // Detaching the tag is successful
    // Update

    const int fileTagIdsSize = files[i].tag_ids.size();
    for (int y = 0; y < fileTagIdsSize; y++) {
      if (files[i].tag_ids[y] == selectedTag->tag_id) {
        files[i].tag_ids.erase(files[i].tag_ids.begin() + y);
        break;
      }
    }

    const int tagsSize = tags.size();
    for (int y = 0; y < tagsSize; y++) {
      if (tags[y].id == selectedTag->tag_id) {
        unattachedTagsListBox->Append(wxString::FromUTF8(tags[y].name),
                                      new TagClientData(selectedTag->tag_id));
        attachedTagsListBox->Delete(tagSel);
        attachedTagsListBox->SetSelection(wxNOT_FOUND);
        detachTagBtn->Disable();
        break;
      }
    }

    break;
  }
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
                        new FileClientData(files[filterMap[i].first].id));
  }

  wxCommandEvent dummy;
  ListBoxChange_State_AttachBtn(dummy);
  ListBoxChange_State_DetachBtn(dummy);
  FileListBoxChange(dummy);
}

void MainFrame::ListBoxChange_State_AttachBtn(wxCommandEvent &event) {
  int fileSel = fileListBox->GetSelection();
  int tagSel = unattachedTagsListBox->GetSelection();
  if (fileSel == wxNOT_FOUND || tagSel == wxNOT_FOUND) {
    attachTagBtn->Disable();
  } else {
    attachTagBtn->Enable();
  }
}

void MainFrame::ListBoxChange_State_DetachBtn(wxCommandEvent &event) {
  int fileSel = fileListBox->GetSelection();
  int tagSel = attachedTagsListBox->GetSelection();
  if (fileSel == wxNOT_FOUND || tagSel == wxNOT_FOUND) {
    detachTagBtn->Disable();
  } else {
    detachTagBtn->Enable();
  }
}

void MainFrame::FileListBoxChange(wxCommandEvent &event) {
  detachTagBtn->Disable();
  attachTagBtn->Disable();
  attachedTagsListBox->Clear();
  unattachedTagsListBox->Clear();

  int fileSel = fileListBox->GetSelection();
  if (fileSel == wxNOT_FOUND) {
    attachedTagsListBox->Disable();
    unattachedTagsListBox->Disable();
    return;
  }

  attachedTagsListBox->Enable();
  unattachedTagsListBox->Enable();

  FileClientData *selectedFile =
      static_cast<FileClientData *>(fileListBox->GetClientObject(fileSel));

  const int filesSize = files.size();
  for (int i = 0; filesSize; i++) {
    if (files[i].id == selectedFile->file_id) {
      // Found the file
      std::vector<int> tagIdsOfFile = files[i].tag_ids;
      const int tagsSize = tags.size();
      for (int y = 0; y < tagsSize; y++) {
        bool tagFound = false;
        for (int z = 0; z < tagIdsOfFile.size(); z++) {
          if (tags[y].id == tagIdsOfFile[z]) {
            attachedTagsListBox->Append(wxString::FromUTF8(tags[y].name),
                                        new TagClientData(tags[y].id));
            tagIdsOfFile.erase(tagIdsOfFile.begin() + z);
            tagFound = true;
            break;
          }
        }
        if (tagFound)
          continue;
        unattachedTagsListBox->Append(wxString::FromUTF8(tags[y].name),
                                      new TagClientData(tags[y].id));
      }
      break;
    }
  }
}
