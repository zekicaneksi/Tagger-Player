#ifndef FILELISTBOX_H
#define FILELISTBOX_H

#include "../logic/logic.h"
#include <wx/listbox.h>

class FileListBox : public wxListBox {
public:
  std::vector<logic::File> *files;

  FileListBox(wxWindow *parent, std::vector<logic::File> *files_arg,
              wxWindowID windowID);
};

class FileClientData : public wxClientData {
public:
  FileClientData(int file_id_arg);

  int file_id;
};

#endif
