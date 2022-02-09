/*****************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  ais-vd Plugin
 *
 ***************************************************************************
 *   Copyright (C) 2010 by 2014 by Dirk Smits                              *
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

#ifndef _AISVDPI_H_
#define _AISVDPI_H_

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers

#include "ocpn_plugin.h"
#include "config.h"

#include "wx/valtext.h"
#include "wx/datectrl.h"
#include "wx/dateevt.h"
#include <wx/fileconf.h>
#include <wx/notebook.h>
#include <wx/timectrl.h>
#include <wx/spinctrl.h>


//#define     PLUGIN_VERSION_MAJOR    0
//#define     PLUGIN_VERSION_MINOR    0

#define     MY_API_VERSION_MAJOR    1
#define     MY_API_VERSION_MINOR    16

//#include "nmea0183/nmea0183.h"
//@begin control identifiers
#define ID_T wxID_ANY
#define ID_PANEL wxID_ANY
#define ID_CHOICE wxID_ANY
#define ID_TEXTCTRL wxID_ANY
#define ID_TEXTCTRL1 wxID_ANY
#define ID_TEXTCTRL2 wxID_ANY
#define ID_COMBCTRL wxID_ANY
#define ID_DATECTRL wxID_ANY
#define ID_NMEACTRL wxID_ANY
#define ID_TIMECTR wxID_ANY
#define ID_BUTTON wxID_ANY
#define ID_BUTTON1 wxID_ANY
#define SYMBOL_T_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_T_TITLE _T("t")
#define SYMBOL_T_IDNAME ID_T
#define SYMBOL_T_SIZE wxSize(500, 500)
#define SYMBOL_T_POSITION wxDefaultPosition
////@end control identifiers

//class TimePickerCtrl;
class aisvd_pi;
class PreferenceDlg;
// An Event handler class to catch events from UI dialog
class aisvd_pi_event_handler : public wxEvtHandler
{
public:

  aisvd_pi_event_handler(aisvd_pi *parent);
  ~aisvd_pi_event_handler();
  void OnReadBtnClick(wxCommandEvent &event);
  void OnSendBtnClick( wxCommandEvent &event );
  void OnDestValSelect(wxCommandEvent &event);
  void OnAnyValueChange(wxCommandEvent &event);
  void OnNavStatusSelect(wxCommandEvent &event);
  aisvd_pi  *m_parent;
};

//----------------------------------------------------------------------------------------------------------
//    The PlugIn Class Definition
//----------------------------------------------------------------------------------------------------------


class aisvd_pi : public opencpn_plugin_116 
{
  public:
    aisvd_pi(void *ppimgr);
    ~aisvd_pi();

//  The required PlugIn Methods
    int Init(void);
    bool DeInit(void);

    int GetAPIVersionMajor();
    int GetAPIVersionMinor();
    int GetPlugInVersionMajor();
    int GetPlugInVersionMinor();
	  wxBitmap *GetPlugInBitmap();
    wxString GetCommonName();
    wxString GetShortDescription();
    wxString GetLongDescription();

// The optional method overrides
  void SetNMEASentence(wxString &sentence);

// The override PlugIn Methods
  void ShowPreferencesDialog( wxWindow* parent );
  void SetPluginMessage(wxString &message_id, wxString &message_body);
	void OnSetupOptions();
  void OnCloseToolboxPanel(int page_sel, int ok_apply_cancel);
  bool SaveConfig( void );
	void UpdateDestVal();
	void UpdateDraught();
	void UpdatePersons();
  void RequestAISstatus();
	void SendSentence();
  void SetSendBtnLabel();
  void UpdateDataFromVSD(wxString &sentence);
  wxString GetShipType(int);

  PreferenceDlg *prefDlg;
  wxArrayString StatusChoiceStrings;
  wxString AIS_type;
  wxString m_Destination;
  wxString m_Draught;
  wxString m_Persons;
  wxString m_InitDest;

private:
  bool LoadConfig( void );  
	wxWindow         *m_parent_window;
	wxBitmap          m_plugin_icon;
  wxScrolledWindow  *m_AIS_VoyDataWin;
  wxString          g_PrivateDataDir;
	aisvd_pi_event_handler *m_event_handler;
	
	wxChoice* StatusChoice;
	wxTextCtrl* m_DestTextCtrl;
  wxComboBox* m_DestComboBox;
	wxTextCtrl* DraughtTextCtrl;
  wxTextCtrl* PersonsTextCtrl;
  wxSpinCtrl *m_pCtrlMonth, *m_pCtrlDay;
  wxSpinCtrl *m_pCtrlHour, *m_pCtrlMinute;
  wxButton* m_SendBtn;
  wxButton* m_BtnReadAIS;
	
	wxFileConfig        *m_pconfig;
	////@begin t member function declarations

    wxString GetDestination() const { return m_Destination ; }
    void SetDestination(wxString value) { m_Destination = value ; }

    wxString GetDraught() const { return m_Draught ; }
    void SetDraught(wxString value) { m_Draught = value ; }

    wxString GetPersons() const { return m_Persons ; }
    void SetPersons(wxString value) { m_Persons = value ; }

    unsigned char ComputeChecksum( wxString sentence ) const;
    /// Retrieves bitmap resources
    //wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    //wxIcon GetIconResource( const wxString& name );
////@end t member function declarations

    /// Should we show tooltips?
    //static bool ShowToolTips();

    //wxBitmap m_panelBitmap;
	
};
///////////////////////////////////////////////////////////////////////////////
/// Class PreferenceDlg
///////////////////////////////////////////////////////////////////////////////
class PreferenceDlg : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText2;
		
		wxStdDialogButtonSizer* m_sdbSizer2;
		wxButton* m_sdbSizer2OK;
		wxButton* m_sdbSizer2Cancel;
	
	public:
		
		PreferenceDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~PreferenceDlg();
		wxChoice* m_choice2;
		wxStringList m_AIS_type_list;
};

#endif
