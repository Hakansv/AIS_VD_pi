/******************************************************************************
 * updated: 11-02-2014
 * Project:  OpenCPN
 * Purpose:  ais-vd Plugin
 * Author:   Dirk Smits
 *
 ***************************************************************************
 *   Copyright (C) 2014 by Dirk Smits                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.         *
 ***************************************************************************
 */


#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#include "ais-vd_pi.h"
#include "default_pi.xpm"
#include "wx/tokenzr.h"

// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr)
{
    return new aisvd_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p)
{
    delete p;
}

//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------

aisvd_pi::aisvd_pi(void *ppimgr)
     :opencpn_plugin_116(ppimgr)
{
  // Create the PlugIn icons
  //m_panelBitmap = wxBitmap(default_pi);
  //m_pplugin_icon = &m_panelBitmap;
  m_pplugin_icon = new wxBitmap(default_pi);

  m_event_handler = new aisvd_pi_event_handler(this);

  // Get and build if necessary a private data dir
  /*g_PrivateDataDir = *GetpPrivateApplicationDataLocation();
  g_PrivateDataDir += wxFileName::GetPathSeparator();
  g_PrivateDataDir += _T("ais-vd_pi");
  g_PrivateDataDir += wxFileName::GetPathSeparator();
  if (!::wxDirExists(g_PrivateDataDir))
    ::wxMkdir(g_PrivateDataDir);*/

  //    Get a pointer to the opencpn configuration object
  m_pconfig = GetOCPNConfigObject();
  //    And load the configuration items
  LoadConfig();
}

aisvd_pi::~aisvd_pi()
{
  delete m_pplugin_icon;
}

int aisvd_pi::Init(void)
{
      AddLocaleCatalog( _T("opencpn-ais-vd_pi") );      

      prefDlg = NULL;
      return ( INSTALLS_TOOLBOX_PAGE | WANTS_NMEA_SENTENCES | WANTS_PREFERENCES | WANTS_CONFIG );
      //return ( WANTS_PREFERENCES | WANTS_NMEA_SENTENCES | WANTS_PLUGIN_MESSAGING | WANTS_CONFIG );
}

bool aisvd_pi::DeInit(void)
{
      //SaveConfig();
      delete m_event_handler;
      if( m_AIS_VoyDataWin )
      {
          if( DeleteOptionsPage( m_AIS_VoyDataWin ) )
              m_AIS_VoyDataWin = NULL;
      }
      
      return true;
}

int aisvd_pi::GetAPIVersionMajor()
{
      return MY_API_VERSION_MAJOR;
}

int aisvd_pi::GetAPIVersionMinor()
{
      return MY_API_VERSION_MINOR;
}

int aisvd_pi::GetPlugInVersionMajor()
{
      return PLUGIN_VERSION_MAJOR;
}

int aisvd_pi::GetPlugInVersionMinor()
{
      return PLUGIN_VERSION_MINOR;
}

wxBitmap *aisvd_pi::GetPlugInBitmap()
{
      return m_pplugin_icon;
}

wxString aisvd_pi::GetCommonName()
{
      return _T("ais-vd");
}

wxString aisvd_pi::GetShortDescription()
{
      return _T("AIS Voyage Data");
}

wxString aisvd_pi::GetLongDescription()
{
      return _T("Set the static voyage data in the AIS class A tranceiver");
}

void aisvd_pi::SetNMEASentence(wxString &sentence)
{
  if (sentence.Mid(0, 1).IsSameAs("$") && sentence.Mid(3, 3).IsSameAs("VSD")) {
    wxString msg = "Passed NMEA multiplexer: ";
    msg.Append(sentence.Mid(0, sentence.Len()-2));
    wxLogMessage(msg);
  }
}


void aisvd_pi::ShowPreferencesDialog( wxWindow* parent )
{
    if (!prefDlg)
        prefDlg = new PreferenceDlg(parent);
    if ( prefDlg->ShowModal() == wxID_OK )
        AIS_type = prefDlg->m_choice2->GetString(prefDlg->m_choice2->GetSelection());

    delete prefDlg;
    prefDlg = NULL;
}

void aisvd_pi::SetPluginMessage(wxString &message_id, wxString &message_body)
{

}
//      Options Dialog Page management
void aisvd_pi::OnSetupOptions(){
   // Set validators
        const wxString AllowDest[] = {
            wxT("a"), wxT("b"), wxT("c"), wxT("d"), wxT("e"), wxT("f"), wxT("g"), wxT("h"), wxT("i"), wxT("j"), wxT("k"), wxT("l"), wxT("m"), wxT("n"), wxT("o"), wxT("p"), wxT("q"), wxT("r"), wxT("s"), wxT("t"), wxT("u"), wxT("v"), wxT("w"), wxT("x"), wxT("y"), wxT("z"), wxT("A"), wxT("B"), wxT("C"), wxT("D"), wxT("E"), wxT("F"), wxT("G"), wxT("H"), wxT("I"), wxT("J"), wxT("K"), wxT("L"), wxT("M"), wxT("N"), wxT("O"), wxT("P"), wxT("Q"), wxT("R"), wxT("S"), wxT("T"), wxT("U"), wxT("V"), wxT("W"), wxT("X"), wxT("Y"), wxT("Z"),wxT(" "), wxT("0"), wxT("1"), wxT("2"), wxT("3"), wxT("4"), wxT("5"), wxT("6"), wxT("7"), wxT("8"), wxT("9"), wxT(":"), wxT(";"), wxT("<"), wxT(">"), wxT("?"), wxT("@"), wxT("["), wxT("]"), wxT("!"), wxT(","), wxT("."), wxT("-"), wxT("="), wxT("*"), wxT("#")};
        wxArrayString* ArrayAllowDest = new wxArrayString(78, AllowDest);
        wxTextValidator DestVal( wxFILTER_INCLUDE_CHAR_LIST, & m_Destination);
        DestVal.SetIncludes(*ArrayAllowDest);
    //m_DestTextCtrl->SetValidator( DestVal);

        const wxString AllowDraught[] = {
            wxT("0"), wxT("1"), wxT("2"), wxT("3"), wxT("4"), wxT("5"), wxT("6"), wxT("7"), wxT("8"), wxT("9"), wxT(".")};
        wxArrayString* ArrayAllowDraught = new wxArrayString(11, AllowDraught);
        wxTextValidator DraughtVal( wxFILTER_INCLUDE_CHAR_LIST, & m_Draught);
        DraughtVal.SetIncludes(*ArrayAllowDraught);
    //DraughtTextCtrl->SetValidator( DraughtVal );

        const wxString AllowPersons[] = {
            wxT("0"), wxT("1"), wxT("2"), wxT("3"), wxT("4"), 
            wxT("5"), wxT("6"), wxT("7"), wxT("8"), wxT("9")};
        wxArrayString* ArrayAllowPersons = new wxArrayString(10, AllowPersons);
        wxTextValidator PersonsVal( wxFILTER_INCLUDE_CHAR_LIST, & m_Persons);
        PersonsVal.SetIncludes(*ArrayAllowPersons);
    //PersonsTextCtrl->SetValidator( PersonsVal );

    //  Create the AISVD Options panel, and load it
    m_AIS_VoyDataWin = AddOptionsPage( PI_OPTIONS_PARENT_SHIPS, _("AIS static") );
    wxStaticBox* itemStaticBoxSizer3Static = new wxStaticBox(m_AIS_VoyDataWin, 
                                             wxID_ANY, _("Set static voyage data to AIS"));
    wxStaticBoxSizer* itemStaticBoxSizer3 = new wxStaticBoxSizer(itemStaticBoxSizer3Static, wxVERTICAL);
    m_AIS_VoyDataWin->SetSizer(itemStaticBoxSizer3);

    wxFlexGridSizer* itemFlexGridSizer4 = new wxFlexGridSizer(0, 2, 0, 0); // 10, 5);
    itemStaticBoxSizer3->Add(itemFlexGridSizer4, 0, wxGROW | wxALL, 20);

    wxStaticText* itemStaticText5 = new wxStaticText(m_AIS_VoyDataWin, wxID_STATIC, 
                                    _("Navigational status"), wxDefaultPosition, wxDefaultSize, 0);
    itemFlexGridSizer4->Add(itemStaticText5, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL, 5);

    wxArrayString StatusChoiceStrings;

    StatusChoiceStrings.Add(_("Underway using engine"));
    StatusChoiceStrings.Add(_("At anchor"));
    StatusChoiceStrings.Add(_("Not under command"));
    StatusChoiceStrings.Add(_("Restricted manoeuverability"));
    StatusChoiceStrings.Add(_("Constrained by draught"));
    StatusChoiceStrings.Add(_("Moored"));
    StatusChoiceStrings.Add(_("Aground"));
    StatusChoiceStrings.Add(_("Engaged in Fishing"));
    StatusChoiceStrings.Add(_("Under way sailing"));
    StatusChoice = new wxChoice(m_AIS_VoyDataWin, ID_CHOICE, wxDefaultPosition,
                                wxDefaultSize, StatusChoiceStrings, 0);
    itemFlexGridSizer4->Add(StatusChoice, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxALL, 5);

    wxStaticText* itemStaticText7 = new wxStaticText(m_AIS_VoyDataWin, wxID_STATIC, 
                                    _("Key a destination"),
                                    wxDefaultPosition, wxDefaultSize, 0);
    itemFlexGridSizer4->Add(itemStaticText7, 0, 
                        wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL, 5);

    wxStaticText* itemStaticText19 = new wxStaticText(m_AIS_VoyDataWin, wxID_STATIC,
                                     _("or select a destination"), 
                                     wxDefaultPosition, wxDefaultSize, 0);
    itemFlexGridSizer4->Add(itemStaticText19, 0, 
                        wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL, 5);

    m_DestTextCtrl = new wxTextCtrl(m_AIS_VoyDataWin, ID_TEXTCTRL, wxEmptyString, 
                                    wxDefaultPosition, wxDefaultSize, 0, DestVal);
    m_DestTextCtrl->SetMaxLength(20);
    itemFlexGridSizer4->Add(m_DestTextCtrl, 0, wxGROW | wxALIGN_CENTER_VERTICAL | wxALL, 5);

    m_DestComboBox = new wxComboBox(m_AIS_VoyDataWin, wxID_ANY, wxEmptyString,
                                    wxDefaultPosition, wxDefaultSize, 0, NULL, 0);
    itemFlexGridSizer4->Add(m_DestComboBox, 0, wxEXPAND | wxTOP, 5);

    wxStaticText* itemStaticText9 = new wxStaticText(m_AIS_VoyDataWin, wxID_STATIC, 
                                    _("Draught (m)"), wxDefaultPosition, wxDefaultSize, 0);
    itemFlexGridSizer4->Add(itemStaticText9, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL, 5);

    DraughtTextCtrl = new wxTextCtrl(m_AIS_VoyDataWin, ID_TEXTCTRL1, wxEmptyString, 
                                     wxDefaultPosition, wxDefaultSize, 0, DraughtVal);
    itemFlexGridSizer4->Add(DraughtTextCtrl, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxALL, 5);

    wxStaticText* itemStaticText11 = new wxStaticText(m_AIS_VoyDataWin, wxID_STATIC, 
                                     _("No. of Persons onboard"), wxDefaultPosition, wxDefaultSize, 0);
    itemFlexGridSizer4->Add(itemStaticText11, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL, 5);

    PersonsTextCtrl = new wxTextCtrl(m_AIS_VoyDataWin, ID_TEXTCTRL2, wxEmptyString, 
                                     wxDefaultPosition, wxDefaultSize, 0, PersonsVal);
    itemFlexGridSizer4->Add(PersonsTextCtrl, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxALL, 5);

    wxStaticText* itemStaticText13 = new wxStaticText(m_AIS_VoyDataWin, wxID_STATIC, 
                                     _("ETA date"), wxDefaultPosition, wxDefaultSize, 0);
    itemFlexGridSizer4->Add(itemStaticText13, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL, 5);

    DatePicker = new wxDatePickerCtrl(m_AIS_VoyDataWin, ID_DATECTRL, wxDateTime(), 
                                      wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT);
    itemFlexGridSizer4->Add(DatePicker, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxALL, 5);

    wxStaticText* itemStaticText15 = new wxStaticText(m_AIS_VoyDataWin, wxID_STATIC, 
                                     _("ETA Time (UTC)"), wxDefaultPosition, wxDefaultSize, 0);
    itemFlexGridSizer4->Add(itemStaticText15, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL, 5);

    TimePickCtrl = new wxTimePickerCtrl(m_AIS_VoyDataWin, ID_TIMECTR, wxDateTime(), wxDefaultPosition, wxDefaultSize, 0);
    itemFlexGridSizer4->Add(TimePickCtrl, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxALL, 5);

    wxBoxSizer* itemBoxSizer17 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer3->Add(itemBoxSizer17, 0, wxGROW | wxALL, 5);
    /*
        wxButton* itemButton18 = new wxButton( m_AIS_VoyDataWin, ID_BUTTON, 
                                _T("Read from AIS"), wxDefaultPosition, wxDefaultSize, 0 );
        itemBoxSizer17->Add(itemButton18, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    */
    wxButton* SendBtn = new wxButton(m_AIS_VoyDataWin, ID_BUTTON1, 
                                     _("Send to AIS"), wxDefaultPosition, wxDefaultSize, 0);
    itemBoxSizer17->Add(SendBtn, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    //  Connect to Events
    m_DestComboBox->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED,
                         wxCommandEventHandler(aisvd_pi_event_handler::OnDestValSelChange),
                           NULL, m_event_handler);

    SendBtn->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                        wxCommandEventHandler(aisvd_pi_event_handler::OnSendBtnClick),
                           NULL, m_event_handler);

    //content construction
    m_AIS_VoyDataWin->Layout();

    //Update values in Controls
    StatusChoice->SetStringSelection(StatusChoiceStrings[0]);
    m_DestTextCtrl->SetValue(m_Destination);
    DraughtTextCtrl->SetValue(m_Draught);
    PersonsTextCtrl->SetValue(m_Persons);
    wxDateTime now = wxDateTime::Now().MakeUTC();
    if (now.IsLaterThan(m_EtaDateTime)) {
      m_EtaDateTime= now.SetSecond(0);
    }
    DatePicker->SetValue(m_EtaDateTime);
    TimePickCtrl->SetValue(m_EtaDateTime);
    m_DestComboBox->Append(">>");
    wxStringTokenizer tkn(m_InitDest, ";");
    while (tkn.HasMoreTokens()) {
      m_DestComboBox->Append(tkn.GetNextToken());
    }
    m_DestComboBox->Select(0);
}

bool aisvd_pi::LoadConfig( void )
{
    wxFileConfig *pConf = (wxFileConfig *) m_pconfig;

    if( pConf ) {
        wxString temp;
        pConf->SetPath( _T("/PlugIns/ais-vd") );
        pConf->Read( _T("Destination"), &m_Destination );
        pConf->Read( _T("Draught"), &m_Draught );
        pConf->Read( _T("Persons"), &m_Persons);
        pConf->Read( _T("Eta"), &temp);
        m_EtaDateTime.ParseDateTime(temp); 
        pConf->Read(_T("DestSelections"), &m_InitDest);
    }

    return true;
}

bool aisvd_pi::SaveConfig( void )
{
    wxFileConfig *pConf = (wxFileConfig *) m_pconfig;

    if( pConf ) {
        pConf->SetPath( _T("/PlugIns/ais-vd") );
        pConf->Write( _T("Destination"), m_Destination );
        pConf->Write( _T("Draught"), m_Draught );
        pConf->Write( _T("Persons"), m_Persons );
        pConf->Write( _T("Eta"), m_EtaDateTime.Format() );
        // Save max 15 dest-selections to config.
        if (m_AIS_VoyDataWin) {
          wxString destarr;
          size_t size(0);
          size = wxMin(15, m_DestComboBox->GetCount());
          for (size_t i = 1; i < size; i++) {
            m_DestComboBox->Select(i);
            destarr << m_DestComboBox->GetValue();
            if (i < size - 1) destarr << ";";
          }
          m_DestComboBox->Select(0);
          if (size) pConf->Write(_T("DestSelections"), destarr);
        }
    }

    return true;
}

void aisvd_pi::OnCloseToolboxPanel(int page_sel, int ok_apply_cancel)
{

}

void aisvd_pi::UpdateDestVal()
{
  if (m_DestComboBox->GetValue() != wxEmptyString &&
      m_DestComboBox->GetValue() != ">>") {
    m_Destination = m_DestComboBox->GetValue();
    //Move last selection to top
    int pos = m_DestComboBox->GetSelection();
    if (pos > 2) {
      m_DestComboBox->Delete(pos);
      m_DestComboBox->Insert(m_Destination, 1);
    }
    m_DestComboBox->Select(0);
  }
  else if(m_DestTextCtrl->GetValue() != wxEmptyString) {
    m_Destination = m_DestTextCtrl->GetValue();
  }
  else return;
  
  m_Destination = m_Destination.MakeUpper();
  m_DestTextCtrl->SetValue( m_Destination );
  //Check if exist else add
  if (wxNOT_FOUND == m_DestComboBox->FindString(m_Destination)) {
    m_DestComboBox->Insert(m_Destination, 1);
  }

}
void aisvd_pi::UpdateDraught()
{
    m_Draught = DraughtTextCtrl->GetValue();
    double temp;
    m_Draught.ToDouble(&temp);
    if ( temp >= 25.5 )
        temp = 25.5;
    m_Draught = wxString::Format(wxT("%2.1f"), temp);
    DraughtTextCtrl->SetValue(m_Draught);
}
void aisvd_pi::UpdatePersons()
{
    m_Persons = PersonsTextCtrl->GetValue();
    long temp;
    m_Persons.ToLong(&temp);
    if ( temp >= 8100 )
        temp = 8100;
    m_Persons = wxString::Format(wxT("%i"), temp);
    PersonsTextCtrl->SetValue(m_Persons);
}
void aisvd_pi::UpdateEta()
{
    m_EtaDateTime = DatePicker->GetValue();
    m_EtaDateTime.SetHour( TimePickCtrl->GetValue().GetHour() );
    m_EtaDateTime.SetMinute( TimePickCtrl->GetValue().GetMinute() );
}
void aisvd_pi::SendSentence()
{
    wxString S;
    S= _T("$ECVSD,"); // EC for Electronic Chart

    //S.Append( wxString::Format(_T("%d,"), 37)); //TODO Type of ship and cargo category
    //We dont send ship type. It will be set by AIS static data and can be password protected by some devices
    S.Append( _T(",") );
    S.Append( m_Draught ); S.Append( _T(",") );
    S.Append( m_Persons ); S.Append( _T(",") );
    S.Append( m_Destination ); S.Append( _T(",") );
    S.Append( wxString::Format(_T("%02d%02d00,"), m_EtaDateTime.GetHour(), 
                               m_EtaDateTime.GetMinute() )); //eta time HHmm
    S.Append( wxString::Format(_T("%d,"), m_EtaDateTime.GetDay() )); // eta Day
    S.Append( wxString::Format(_T("%d,"), m_EtaDateTime.GetMonth()+1 )); // eta Month
    S.Append( wxString::Format(_T("%d,"), StatusChoice->GetSelection() )); //Navigation status
    S.Append( wxString::Format(_T("%d"), 0 )); // TODO Regional application flags, 0 to 15
    S.Append( _T("*")); // End data
    S.Append( wxString::Format(_T("%02X"), ComputeChecksum(S) ));
    S += _T("\r\n");
    //wxPuts(S);
    PushNMEABuffer(S); //finaly send NMEA string
    //wxString msg = "Voyage data sent to NMEA buffer: ";
    //msg.Append(S.Mid(0, S.Len() - 4));
    //wxLogMessage(msg);
}

unsigned char aisvd_pi::ComputeChecksum( wxString sentence ) const
{
    unsigned char calculated_checksum = 0;
    for(wxString::const_iterator i = sentence.begin()+1; i != sentence.end() && *i != '*'; ++i)
        calculated_checksum ^= static_cast<unsigned char> (*i);

   return( calculated_checksum );
}


//The preference dialog
PreferenceDlg::PreferenceDlg( wxWindow* parent, wxWindowID id, const wxString& title, 
                             const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* bSizer2;
    bSizer2 = new wxBoxSizer( wxVERTICAL );

    wxGridSizer* gSizer2;
    gSizer2 = new wxGridSizer( 0, 2, 0, 0 );

    m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Type of AIS"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText2->Wrap( -1 );
    gSizer2->Add( m_staticText2, 0, wxALL, 5 );

    wxString m_choice2Choices[] = { wxT("Class A Transponder supporting NMEA0183 $ECVSD") };
    int m_choice2NChoices = sizeof( m_choice2Choices ) / sizeof( wxString );
    m_choice2 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice2NChoices, m_choice2Choices, 0 );
    m_choice2->SetSelection( 0 );
    gSizer2->Add( m_choice2, 0, wxALL, 5 );

    bSizer2->Add( gSizer2, 1, wxEXPAND, 5 );

    m_sdbSizer2 = new wxStdDialogButtonSizer();
    m_sdbSizer2OK = new wxButton( this, wxID_OK );
    m_sdbSizer2->AddButton( m_sdbSizer2OK );
    m_sdbSizer2Cancel = new wxButton( this, wxID_CANCEL );
    m_sdbSizer2->AddButton( m_sdbSizer2Cancel );
    m_sdbSizer2->Realize();

    bSizer2->Add( m_sdbSizer2, 1, wxEXPAND, 5 );

    this->SetSizer( bSizer2 );
    this->Layout();
    bSizer2->Fit( this );

    this->Centre( wxBOTH );
}

PreferenceDlg::~PreferenceDlg()
{
}

aisvd_pi_event_handler::aisvd_pi_event_handler(aisvd_pi *parent)
{
    m_parent = parent;
}

aisvd_pi_event_handler::~aisvd_pi_event_handler()
{
}

void aisvd_pi_event_handler::OnSendBtnClick( wxCommandEvent &event )
{
    m_parent->UpdateDestVal();
    m_parent->UpdateDraught();
    m_parent->UpdatePersons();
    m_parent->UpdateEta();
    m_parent->SaveConfig();
    m_parent->SendSentence();
}

void aisvd_pi_event_handler::OnDestValSelChange(wxCommandEvent &event) {
  m_parent->UpdateDestVal();
}
