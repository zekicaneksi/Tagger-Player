#ifndef FILELISTBOX_H
#define FILELISTBOX_H

#include <wx/listbox.h>
#include "../logic/logic.h"

class FileListBox : public wxListBox {
public:
    FileListBox(wxWindow * parent);

    void OnDblClick(wxCommandEvent& event);
};

class FileClientData: public wxClientData {
public:
    FileClientData(logic::File file_arg);

    logic::File file;
};

#endif