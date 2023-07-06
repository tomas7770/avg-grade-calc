#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/listctrl.h>
#include <wx/spinctrl.h>
#include <wx/settings.h>
#include <vector>
#include <algorithm>

class AvgGradeCalc : public wxApp {
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame {
public:
    MyFrame();
};

enum {
    ID_ADD = 1,
    ID_REMOVE,
    ID_UP,
    ID_DOWN,
    ID_APPLY,
    ID_LIST,
};

struct Entry {
    int value, weight;
};

wxIMPLEMENT_APP(AvgGradeCalc);

std::vector<Entry> entries;

double calculateAverage() {
    int sum = 0, sumWeights = 0;
    for (Entry entry: entries) {
        sum += entry.value*entry.weight;
        sumWeights += entry.weight;
    }
    return ((double) sum)/((double) sumWeights);
}

void updateAverageText(wxStaticText* averageText) {
    averageText->SetLabelText("Average: " + wxString::Format(wxT("%f"), calculateAverage()));
}

bool AvgGradeCalc::OnInit() {
    MyFrame *frame = new MyFrame();
    frame->Show(true);
    return true;
}

MyFrame::MyFrame() : wxFrame(nullptr, wxID_ANY, "Average Grade Calculator") {
    Centre();
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_FRAMEBK));

    wxBoxSizer* horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
    wxListView* list = new wxListView(this, ID_LIST, wxDefaultPosition, wxDefaultSize, 32L | wxLC_SINGLE_SEL);

    horizontalSizer->Add(list, 1, wxEXPAND | wxALL, FromDIP(10));
    list->AppendColumn("Course");
    list->AppendColumn("Grade");
    list->AppendColumn("Weight");

    wxBoxSizer* controlSizer = new wxBoxSizer(wxVERTICAL);
    horizontalSizer->Add(controlSizer, 0, wxALL, FromDIP(10));

    wxBoxSizer* buttonContainer = new wxBoxSizer(wxHORIZONTAL);
    buttonContainer->Add(new wxButton(this, ID_ADD, "+", wxDefaultPosition, FromDIP(wxSize(40, -1))), 0, wxRIGHT, FromDIP(10));
    buttonContainer->Add(new wxButton(this, ID_REMOVE, "-", wxDefaultPosition, FromDIP(wxSize(40, -1))), 0, wxRIGHT, FromDIP(10));
    buttonContainer->Add(new wxButton(this, ID_UP, "Up", wxDefaultPosition, FromDIP(wxSize(50, -1))), 0, wxRIGHT, FromDIP(10));
    buttonContainer->Add(new wxButton(this, ID_DOWN, "Down", wxDefaultPosition, FromDIP(wxSize(50, -1))));
    controlSizer->Add(buttonContainer, 0, wxBOTTOM, FromDIP(20));

    wxCheckBox* reselectRemoveBox = new wxCheckBox(this, wxID_ANY, "Select next after removal");
    controlSizer->Add(reselectRemoveBox, 0, wxBOTTOM, FromDIP(20));

    wxFlexGridSizer* colControlContainer = new wxFlexGridSizer(3, 2, FromDIP(wxSize(10, 10)));

    auto keyText = new wxTextCtrl(this, wxID_ANY);
    colControlContainer->Add(new wxStaticText(this, wxID_ANY, "Course"), 1);
    colControlContainer->Add(keyText, 1);

    auto gradeCtrl = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
        wxDefaultSize, wxALIGN_RIGHT | wxSP_ARROW_KEYS);
    gradeCtrl->SetRange(0, 20);
    colControlContainer->Add(new wxStaticText(this, wxID_ANY, "Grade"), 1);
    colControlContainer->Add(gradeCtrl, 1, wxEXPAND);

    auto weightCtrl = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
        wxDefaultSize, wxALIGN_RIGHT | wxSP_ARROW_KEYS);
    colControlContainer->Add(new wxStaticText(this, wxID_ANY, "Weight"), 1);
    colControlContainer->Add(weightCtrl, 1, wxEXPAND);

    controlSizer->Add(colControlContainer, 0, wxBOTTOM, FromDIP(20));

    auto applyButton = new wxButton(this, ID_APPLY, "Apply");
    applyButton->Enable(false);
    controlSizer->Add(applyButton, 0, wxALIGN_CENTER | wxBOTTOM, FromDIP(20));

    auto averageText = new wxStaticText(this, wxID_ANY, "Average: N/A");
    controlSizer->Add(averageText, 0, wxALIGN_CENTER);

    Bind(wxEVT_BUTTON, [=](wxCommandEvent&) {
        list->InsertItem(0, keyText->GetValue());
        int grade = gradeCtrl->GetValue(), weight = weightCtrl->GetValue();
        list->SetItem(0, 1, wxString::Format(wxT("%d"), grade));
        list->SetItem(0, 2, wxString::Format(wxT("%d"), weight));
        entries.insert(entries.begin(), Entry{grade, weight});
        updateAverageText(averageText);
    }, ID_ADD);

    Bind(wxEVT_BUTTON, [=](wxCommandEvent&) {
        long prev_i = -1;
        for (long i = list->GetFirstSelected(); i != -1; prev_i = i, i = list->GetFirstSelected()) {
            list->DeleteItem(i);
            entries.erase(entries.begin() + i);
        }
        if (reselectRemoveBox->IsChecked() && prev_i != -1)
            list->Select(std::min((unsigned long long) prev_i, entries.size()-1));
        updateAverageText(averageText);
    }, ID_REMOVE);

    Bind(wxEVT_BUTTON, [=](wxCommandEvent&) {
        long i = list->GetFirstSelected();
        if (i <= 0)
            return;

        iter_swap(entries.begin() + i, entries.begin() + i - 1);

        std::vector<wxString> colValues;

        list->Freeze(); 
        wxListItem item; 
        item.SetId(i);
        list->GetItem(item);
        for(int c = 0; c < list->GetColumnCount(); c++) {
            colValues.push_back(list->GetItemText(i, c));
        }
        list->DeleteItem(i);
        item.SetId(i - 1);
        long j = list->InsertItem(item);
        for(int c = 0; c < list->GetColumnCount(); c++) {
            list->SetItem(j, c, colValues.at(c));
        }
        list->Select(i - 1);
        list->Thaw();
    }, ID_UP);

    Bind(wxEVT_BUTTON, [=](wxCommandEvent&) {
        long i = list->GetFirstSelected();
        if (i >= entries.size() - 1)
            return;

        iter_swap(entries.begin() + i, entries.begin() + i + 1);

        std::vector<wxString> colValues;

        list->Freeze(); 
        wxListItem item; 
        item.SetId(i);
        list->GetItem(item);
        for(int c = 0; c < list->GetColumnCount(); c++) {
            colValues.push_back(list->GetItemText(i, c));
        }
        list->DeleteItem(i);
        item.SetId(i + 1);
        long j = list->InsertItem(item);
        for(int c = 0; c < list->GetColumnCount(); c++) {
            list->SetItem(j, c, colValues.at(c));
        }
        list->Select(i + 1);
        list->Thaw();
    }, ID_DOWN);

    Bind(wxEVT_LIST_ITEM_SELECTED, [=](wxListEvent& event) {
        long i = list->GetFirstSelected();
        if (i == -1)
            return;
        keyText->SetValue(list->GetItemText(i, 0));
        gradeCtrl->SetValue(entries.at(i).value);
        weightCtrl->SetValue(entries.at(i).weight);
        applyButton->Enable(true);
    }, ID_LIST);

    Bind(wxEVT_LIST_ITEM_DESELECTED, [=](wxListEvent& event) {
        applyButton->Enable(false);
    }, ID_LIST);

    Bind(wxEVT_BUTTON, [=](wxCommandEvent&) {
        long i = list->GetFirstSelected();
        if (i == -1)
            return;

        list->SetItem(i, 0, keyText->GetValue());
        int grade = gradeCtrl->GetValue(), weight = weightCtrl->GetValue();
        list->SetItem(i, 1, wxString::Format(wxT("%d"), grade));
        list->SetItem(i, 2, wxString::Format(wxT("%d"), weight));
        entries.at(i).value = grade;
        entries.at(i).weight = weight;
        updateAverageText(averageText);
    }, ID_APPLY);

    SetSizerAndFit(horizontalSizer);
}
