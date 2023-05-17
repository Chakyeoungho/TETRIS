#ifndef _EASY_WIN32_H_
#define _EASY_WIN32_H_

// �� ���̺귯���� C ���� Win32 ���α׷����� ���� �Ҽ� �ֵ��� �����ֱ� ���� ��������ϴ�.
// �� ����� ����ؼ� C �� ���鼭 Win32 ���α׷������� �ǽ��� �ϸ� �˴ϴ�.
//
// ������ : �輺�� (tipsware@naver.com, https://blog.naver.com/tipsware, https://cafe.naver.com/easywin32)
//
// ���� ���� : 2019�� 9�� 3�� ȭ����
// �ֱ� ������Ʈ : 2023�� 4�� 3�� ������ (���� ���� ������Ʈ, ������ ���Ͽ� ���缭 ������Ʈ �ϳ׿�. ���� �����մϴ�.)
//
// �� ���̺귯���� ���۱��� '(��)��������'�� �ֽ��ϴ�.
// �� ���̺귯���� C �� �����ϴ� ������� ���� ����������ϴ�.
// ���� ��ó�� ��Ȯ�ϰ� �����ָ� ��� ��Ȳ���� ����� ����� �����մϴ�.
// �� ���̺귯���� �Ŀ� �Ͻô� �е��� �������� ��� �����ϰ� �ֽ��ϴ�.
// �Ŀ��� �����Ͻ� �е��� �Ʒ��� ��ũ�� �����ϼ���.
// https://blog.naver.com/tipsware/221552898585
// �������� ���� ������ ���߿� ū ���� �˴ϴ�. �����մϴ�!!


typedef void (*FP_MOUSE_MSG)(int a_mixed_key, POINT a_pos);
typedef void (*FP_COMMAND)(INT32 a_ctrl_id, INT32 a_notify_code, void *ap_ctrl);
typedef void (*FP_DESTROY)();
typedef int (*FP_USER_MESSAGE)(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
  
struct TargetData
{
	HWND h_wnd, h_base_wnd;
	int win_x, win_y, win_cx, win_cy;
	int x, y, cx, cy;
};

#define MAX_ANI_IMAGE_COUNT     32    // �ִϸ��̼��� ���� �ִ� �̹��� ����

typedef struct TW_AnimationImageData
{
	// �ִϸ��̼��� �����ϴ� �̹����� ������ ���� ������� �̹����� ��ġ
	UINT16 image_count, index;
	// �ִϸ��̼ǿ� ����� �̹����� ���
	void *p_image[MAX_ANI_IMAGE_COUNT];
} TW_AID;

#define IMAGE_BMP   0
#define IMAGE_JPEG  1
#define IMAGE_GIF   2
#define IMAGE_TIFF  3
#define IMAGE_PNG   4

#define PI     3.141592
#define H_PI   1.570796

#define GDI_COLOR(c) ((c >> 16) & 0x000000FF) | (c & 0x0000FF00) | ((c << 16) & 0x00FF0000)

class SystemMessageMap
{
protected:
	void *mp_app_data;
	FP_MOUSE_MSG mp_left_down, mp_left_up, mp_mouse_move;
	FP_COMMAND mp_command;
	FP_DESTROY mp_destroy;
	FP_USER_MESSAGE mp_user_message;

public:
	SystemMessageMap(FP_MOUSE_MSG ap_left_down, FP_MOUSE_MSG ap_left_up, FP_MOUSE_MSG ap_mouse_move, FP_COMMAND ap_command, FP_DESTROY ap_destroy, FP_USER_MESSAGE ap_user_message);
	virtual ~SystemMessageMap();

	void SetAppData(void *ap_data, int a_data_size);
	void *GetAppData();

	FP_MOUSE_MSG GetLeftBtnDown();
	FP_MOUSE_MSG GetLeftBtnUp();
	FP_MOUSE_MSG GetMouseMove();
	FP_COMMAND GetCommand();
	FP_DESTROY GetDestroy();
	FP_USER_MESSAGE GetUserMessage();
};

namespace EasyAPI_Tipsware
{
	void Rectangle(RECT *ap_rect);
	void Rectangle(int a_sx, int a_sy, int a_ex, int a_ey);
	void Rectangle(int a_sx, int a_sy, int a_ex, int a_ey, COLORREF a_color);
	void Rectangle(int a_sx, int a_sy, int a_ex, int a_ey, COLORREF a_line_color, COLORREF a_fill_color);

	void Ellipse(int a_sx, int a_sy, int a_ex, int a_ey);
	void Ellipse(int a_sx, int a_sy, int a_ex, int a_ey, COLORREF a_color);
	void Ellipse(int a_sx, int a_sy, int a_ex, int a_ey, COLORREF a_line_color, COLORREF a_fill_color);

	void TextOut(int a_x, int a_y, const char *ap_format, ...);
	void TextOut(int a_x, int a_y, COLORREF a_color, const char *ap_format, ...);
	void TextCenterOut(RECT *ap_rect, const char *ap_format, ...);
	void DrawText(RECT *ap_rect, int a_text_align, const char *ap_format, ...);

	void Line(int a_sx, int a_sy, int a_ex, int a_ey);
	void Line(int a_sx, int a_sy, int a_ex, int a_ey, COLORREF a_color, int a_tick = 1, int a_style = PS_SOLID);
	void MoveTo(int a_x, int a_y);
	void LineTo(int a_x, int a_y);
	void Pie(int a_left, int a_top, int a_right, int a_bottom, int a_xr1, int a_yr1, int a_xr2, int a_yr2);
	void PieRadian(int a_left, int a_top, int a_right, int a_bottom, double a_s_radian, double a_e_radian);
	void PieDegree(int a_left, int a_top, int a_right, int a_bottom, int a_s_degree, int a_e_degree);

	void GetIntPosFromCircle(int a_center_x, int a_center_y, int a_radius, double a_radian, int *ap_x, int *ap_y);
	void GetIntPosFromCircle(int a_center_x, int a_center_y, int a_radius, int a_degree, int *ap_x, int *ap_y);

	void GetPosFromCircle(double a_center_x, double a_center_y, double a_radius, double a_radian, double *ap_x, double *ap_y);
	void GetPosFromCircle(double a_center_x, double a_center_y, double a_radius, int a_degree, double *ap_x, double *ap_y);

	void MakeArrowHeadPos(POINT a_start_pos, POINT a_end_pos, POINT *ap_pos_list, int a_head_size = 15);
	void DrawArrow(POINT a_start_pos, POINT a_end_pos, int a_type, int a_head_size = 15);
	void DrawArrow(int a_sx, int a_sy, int a_ex, int a_ey, int a_type, int a_head_size = 15);

	HGDIOBJ SelectStockObject(int a_stock_index);
	HGDIOBJ SelectPenObject(COLORREF a_color, int a_tick = 1, int a_style = PS_SOLID);
	HGDIOBJ SelectBrushObject(COLORREF a_color);
	HGDIOBJ SelectFontObject(const char *ap_name, short a_size, unsigned char a_style = 0);

	void SetSimpleColorMode(char a_use_pen = 1, char a_use_brush = 1);
	void ChangePenColor(COLORREF a_color);
	void ChangeBrushColor(COLORREF a_color);
	void ChangeObjectColor(COLORREF a_color);

	void SetTextColor(COLORREF a_color);

	void UseDisplay(int a_index, int a_cx = 800, int a_cy = 600, HWND ah_wnd = NULL);
	void SetWindowStartPos(int a_x, int a_y);
	void ChangeWorkSize(int a_cx, int a_cy, int a_auto_change_display_size = 1);
	void ShowDisplay(int a_index = 0);

	void *FindControl(int a_id);
	void *CreateButton(const char *ap_name, int a_x, int a_y, int a_width, int a_height, int a_id, int a_style = 0, int a_ex_syle = 0);
	void *CreateEdit(int a_x, int a_y, int a_width, int a_height, int a_id, int a_style, int a_ex_syle = 0);
	void *CreateListBox(int a_x, int a_y, int a_width, int a_height, int a_id, void *afp_draw = NULL, int a_style = 0, int a_ex_syle = 0);
	void *CreateComboBox(int a_x, int a_y, int a_width, int a_height, int a_id, void *afp_draw = NULL, int a_style = 0, int a_ex_syle = 0);
	void DestroyControl(void *ap_ctrl);
	void ChangeCtrlColor(void *ap_ctrl, COLORREF a_fill_color, COLORREF a_out_border_color, COLORREF a_in_border_color, COLORREF a_text_color);
	void ChangeEditColor(void *ap_ctrl, COLORREF a_fill_color, COLORREF a_text_color);
	void SetCtrlName(void *ap_ctrl, const char *ap_name);
	void GetCtrlName(void *ap_ctrl, char *ap_name, int a_limit_size);
	void ShowControl(void *ap_ctrl, int a_show_type);
	HWND GetCtrlHandle(void *ap_ctrl);
	void SetIntDataToControl(int a_ctrl_id, int a_value);
	int GetIntDataFromControl(int a_ctrl_id);
	void Invalidate(void *ap_ctrl, int a_bk_flag = 1);
	void SetCtrlFont(void *ap_ctrl, const char *ap_font_name, int a_font_size, int a_attr = 0);
	void SetFocus(void *ap_ctrl);
	void CallDrawItem(int a_id, LPARAM lParam);
	void CallMeasureItem(int a_id, LPARAM lParam);
	INT32 CallCtrlColor(WPARAM wParam, LPARAM lParam);

	void ListBox_ChangeDrawFunc(void *ap_ctrl, void *afp_draw);
	int ListBox_AddString(void *ap_ctrl, const char *ap_string, UINT8 a_auto_select = 1);
	int ListBox_InsertString(void *ap_ctrl, INT32 a_index, const char *ap_string, UINT8 a_auto_select = 1);
	int ListBox_PrintFormat(void *ap_ctrl, const char *ap_format, ...);
	int ListBox_GetTextLength(void *ap_ctrl, INT32 a_index);
	int ListBox_GetText(void *ap_ctrl, INT32 a_index, char *ap_string, int a_max_len);
	void ListBox_SetCurSel(void *ap_ctrl, INT32 a_index);
	INT32 ListBox_GetCurSel(void *ap_ctrl);
	INT32 ListBox_GetCount(void *ap_ctrl);
	void ListBox_DeleteString(void *ap_ctrl, INT32 a_index);
	void ListBox_ResetContent(void *ap_ctrl);
	void ListBox_SetItemDataPtr(void *ap_ctrl, INT32 a_index, void *ap_data);
	void ListBox_SetItemDataPtrEx(void *ap_ctrl, INT32 a_index, const char *ap_string, void *ap_data, UINT8 a_auto_select = 1);
	void *ListBox_GetItemDataPtr(void *ap_ctrl, INT32 a_index);
	void ListBox_SetItemData(void *ap_ctrl, INT32 a_index, INT32 a_data);
	void ListBox_SetItemDataEx(void *ap_ctrl, INT32 a_index, const char *ap_string, INT32 a_data, UINT8 a_auto_select = 1);
	INT32 ListBox_GetItemData(void *ap_ctrl, INT32 a_index);
	INT32 ListBox_FindString(void *ap_ctrl, INT32 a_index, const char *ap_string);
	INT32 ListBox_FindStringExact(void *ap_ctrl, INT32 a_index, const char *ap_string);
	void ListBox_SetItemWidth(void *ap_ctrl, INT32 a_width);
	void ListBox_SetItemHeight(void *ap_ctrl, INT32 a_height);

	void ComboBox_ChangeDrawFunc(void *ap_ctrl, void *afp_draw);
	int ComboBox_AddString(void *ap_ctrl, const char *ap_string, UINT8 a_auto_select = 1);
	int ComboBox_InsertString(void *ap_ctrl, INT32 a_index, const char *ap_string, UINT8 a_auto_select = 1);
	int ComboBox_PrintFormat(void *ap_ctrl, const char *ap_format, ...);
	int ComboBox_GetTextLength(void *ap_ctrl, INT32 a_index);
	int ComboBox_GetText(void *ap_ctrl, INT32 a_index, char *ap_string, int a_max_len);
	void ComboBox_SetCurSel(void *ap_ctrl, INT32 a_index);
	INT32 ComboBox_GetCurSel(void *ap_ctrl);
	INT32 ComboBox_GetCount(void *ap_ctrl);
	void ComboBox_DeleteString(void *ap_ctrl, INT32 a_index);
	void ComboBox_ResetContent(void *ap_ctrl);
	void ComboBox_SetItemDataPtr(void *ap_ctrl, INT32 a_index, void *ap_data);
	void ComboBox_SetItemDataPtrEx(void *ap_ctrl, INT32 a_index, const char *ap_string, void *ap_data, UINT8 a_auto_select = 1);
	void *ComboBox_GetItemDataPtr(void *ap_ctrl, INT32 a_index);
	void ComboBox_SetItemData(void *ap_ctrl, INT32 a_index, INT32 a_data);
	void ComboBox_SetItemDataEx(void *ap_ctrl, INT32 a_index, const char *ap_string, INT32 a_data, UINT8 a_auto_select = 1);
	INT32 ComboBox_GetItemData(void *ap_ctrl, INT32 a_index);
	INT32 ComboBox_FindString(void *ap_ctrl, INT32 a_index, const char *ap_string);
	INT32 ComboBox_FindStringExact(void *ap_ctrl, INT32 a_index, const char *ap_string);
	void ComboBox_SetItemHeight(void *ap_ctrl, INT32 a_height);

	// ����Ʈ�� �Էµ� ���̸� ��� �Լ�
	UINT32 Edit_GetLength(void *ap_ctrl);  
	// ����Ʈ�� ��濡 ���ڿ��� ����ϴ� �Լ�
	void Edit_SetBgTipText(void *ap_ctrl, const char *ap_text, int a_text_len, COLORREF a_color);
	// ����Ʈ ��濡 ���� ���ڿ��� ����� �Լ�
	void Edit_ResetTipText(void *ap_ctrl);
	// ������ �ؽ�Ʈ ���Ͽ��� ���ڿ��� �о� ����Ʈ�� ǥ���ϴ� �Լ�
	INT32 Edit_ReadTextFromFile(void *ap_ctrl, const char *ap_file_path);
	// ����Ʈ�� �Էµ� ���ڿ��� ������ �ؽ�Ʈ�� �����ϴ� �Լ�
	INT32 Edit_SaveTextToFile(void *ap_ctrl, const char *ap_file_path);

	void EnableEnterKey(void *ap_ctrl, WNDPROC afp_filter = NULL);
	void Clear(int a_index = 0, COLORREF a_color = RGB(255, 255, 255));

	void SetAppData(void *ap_data, int a_data_size);
	void *GetAppData();

	HDC GetCurrentDC();
	HBITMAP GetCurrentBitmap();
	void *GetMemDC(int a_index);
	void *GetCurrentMemDC();
	void BitBltByIndex(int a_dest_index, int a_dest_x, int a_dest_y, int a_dest_width, int a_dest_height, int a_src_index, int a_src_x = 0, int a_src_y = 0);
	void BitBltByIndex(int a_dest_x, int a_dest_y, int a_src_index, int a_src_x = 0, int a_src_y = 0);

	void SetTimer(UINT a_timer_id, UINT a_elapse, void *ap_timer_proc);
	void SetTimer(UINT ah_wnd, UINT a_timer_id, UINT a_elapse, void *ap_timer_proc);

	void KillTimer(UINT a_timer_id);
	void KillTimer(UINT ah_wnd, UINT a_timer_id);

	// ...GP �� GDI+ ���� �Լ����Դϴ�!
	// ap_image_path ��ο� �ִ� �̹��� ������ �д� �Լ�!
	void *LoadImageGP(const char *ap_image_path, UINT8 a_is_alpha_fix = 0, UINT8 a_alpha_limit = 254);
	// ap_image�� ����� �̹��� ������ ap_image_path��ο� a_image_type ������ �̹��� ���Ϸ� �����մϴ�.
	void SaveImageGP(void *ap_image, const char *ap_image_path, int a_image_type = IMAGE_PNG);
	void *CreateBitmapGP(int a_width, int a_height, DWORD a_flag = 0, int a_bpp = 32);
	void DeleteImageGP(void *ap_image);

	void DrawImageGP(void *ap_image, int a_x, int a_y);
	void DrawImageGP(void *ap_image, int a_x, int a_y, int a_width, int a_height);
	void DrawImageGP(void *ap_image, int a_x, int a_y, double a_cx_rate, double a_cy_rate);
	void TransparentDrawGP(void *ap_image, int a_x, int a_y, COLORREF a_remove_color);
	void TransparentDrawGP(void *ap_image, int a_x, int a_y, int a_width, int a_height, COLORREF a_remove_color);
	void TransparentDrawGP(void *ap_image, int a_x, int a_y, double a_cx_rate, double a_cy_rate, COLORREF a_remove_color);

	// ������ ��ο� �ִ� ���� �迭�� �̹��� ������ �д� �Լ�
	// ".\\image\\run_00.png", ".\\image\\run_01.png", ".\\image\\run_02.png" ������
	// �а� �ʹٸ� LoadImageAID(L".\\image\\run_", 3); �̶�� ȣ���Ѵ�.
	// ���� Ȯ���ڴ� .png�� �����ϰ� ���� ��ȣ�� 00, 01, ... ������ �����ؾ� �Ѵ�.
	void LoadImageAID(TW_AID *ap_data, const char *ap_path, UINT16 a_count);
	// �Ҵ�� �̹����� ��� �����ϴ� �Լ� (ȣ������ �ʾƵ� ���α׷� ����ÿ� �ڵ����� ���ŵ˴ϴ�.)
	void DeleteImageAID(TW_AID *ap_data);
	// ���� ��ġ�� �̹����� ah_dc�� ���(������ ��� �Ǵ� Ȯ��)�ϰ� ���� �̹��� ��ġ�� �̵��ϴ� �Լ�
	UINT16 DrawImageAID(TW_AID *ap_data, int a_x, int a_y, double a_cx_rate, double a_cy_rate);
	// ������ ��ġ�� �̹����� ah_dc�� ���(������ ��� �Ǵ� Ȯ��)�ϴ� �Լ�
	void DrawImageByIndexAID(TW_AID *ap_data, int a_x, int a_y, int a_index, double a_cx_rate, double a_cy_rate);

	HDC GetDCGP(void *ap_image);
	HBITMAP GetBitmapGP(void *ap_image);
	int GetWidthGP(void *ap_image);
	int GetHeightGP(void *ap_image);
	int GetBppGP(void *ap_image);
	int GetPitchGP(void *ap_image);
	void *GetBitsGP(void *ap_image);

	void *CaptureScreenGP(int a_sx, int a_sy, int a_ex, int a_ey);
	void *CaptureScreenGP();

	void StartSocketSystem();
	void StopSocketSystem();

	// [[[ ���� ���� ���� ]]]
	void *CreateServerSocket(unsigned int a_user_data_size, void *ap_accept_user, void *ap_proc_net_msg, void *ap_close_user,
								int a_index = 0, int a_max_user_count = 100, unsigned char a_key = 0x51);
	void DeleteServerSocket(void *ap_object);
	void DeleteServerSockets();
	int GetServerSocketIndex(void *ap_object);

	void *GetUsersData(void *ap_object);
	void *GetUserDataByHandle(void *ap_object, unsigned int ah_socket);
	void *GetUserDataByIndex(void *ap_object, int a_index);

	void StartListenService(void *ap_object, const char *ap_ip_address, int a_port);
	void SendFrameDataToClient(void *ap_object, unsigned int ah_socket, char a_msg_id, const void *ap_data, int a_size);
	void BroadcastFrameData(void *ap_object, char a_msg_id, const void *ap_data, int a_size);
	void *GetProcessServerData(void *ap_object);
	void CloseSocketInServer(void *ap_object, unsigned int ah_socket, char *ap_close_user);
	void DestroySocketsInServer(void *ap_object);

	// [[[ Ŭ���̾�Ʈ ���� ���� ]]]
	void *CreateClientSocket(void *ap_connect, void *ap_net_msg, void *ap_close, int a_index = 0, unsigned char a_key = 0x51);
	int GetClientSocketIndex(void *ap_object);
	void DeleteClientSocket(void *ap_object);
	void DeleteClientSockets();
	void *GetProcessClientData(void *ap_object);
	int IsConnect(void *ap_object);
	void ConnectToServer(void *ap_object, const char *ap_ip_address, int a_port);
	void CloseSocket(void *ap_object);
	void SendFrameDataToServer(void *ap_object, char a_msg_id, const void *ap_data, int a_size);

	// [[ ���ڿ� ó�� ���� �Ϲ� ��ƿ��Ƽ ]]
	char *GetNextString(char *ap_src_str, char a_delimiter, char *ap_buffer);
	char *GetNextStringEx(char *ap_src_str, char a_delimiter, char *ap_buffer, char a_remove_prefix = ' ');
	char* ConvertTabToSpace(const char* ap_src_string);
	// UTF8 ����� ���ڿ��� ASCII ������ ���ڿ��� ��ȯ�ϴ� �Լ�.
	// �� �Լ��� ���������� �޸𸮸� �Ҵ��ؼ� ap_dest_ascii_str �����Ϳ� �ּҸ� ������ ��
	// ���� �� �Լ��� ����ϴ� �����ڰ� free �Լ��� ����Ͽ� ap_dest_ascii_str �ּҸ� �����ؾ���!
	// �� �Լ��� ��ȯ ���� ��ȯ�� ���ڿ��� �����Դϴ�. (NULL ���� ���� ����)
	int Utf8ToAscii(char **ap_dest_ascii_str, char *ap_src_utf8_str);

	// [[ ���� ���� �м��� ���� ]]
	void *CreateExpParsor();
	const char *GetExpString(void *ap_parsor);
	void InitExpParser(void *ap_parsor);
	void SetExpString(void *ap_parsor, const char *ap_string);
	void RunExpParsing(void *ap_parsor);   // ����ڰ� ������ ������ �м��Ѵ�

	int GetExpVariableCount(void *ap_parsor);
	const char *GetExpVariableName(void *ap_parsor, int a_index);
	// ������ ������ ���� �����Ѵ�.
	void SetExpVarValue(void *ap_parsor, int a_index, double a_value);
	// ������ ������ ������ ���� �ִ� ��ġ�� �ּҷ� �˷��ش�
	void SetExpVariablePointer(void *ap_parsor, int a_index, double *ap_data);
	// �������� �ּҷ� �����ϵ��� �Ǿ� �ִ� ������ �ִٸ� �ش� �ּҿ��� ���� �о� �������� ������Ʈ �Ѵ�.
	void UpdateExpVariableValue(void *ap_parsor);
	// �̹� ������ parsing�Ǿ��ٸ� �ٽ� parsing�� �ʿ䰡 ���� ���Լ��� ȣ������, ExecExp �Լ��� ȣ���ϸ� �ȴ�
	char RestoreExpParserInfo(void *ap_parsor);
	char GetExpReadyFlag(void *ap_parsor);   // Parsing �� �Ǿ��ִ����� �˷��ش� ( 0 -> �Ľ̾ȵ�, 1 -> �Ľ̵� )
	double ExecExp(void *ap_parsor, unsigned int a_prev_depth = 0);  // Parsing�� ������ �����Ͽ� ������ ������� ��´�
	void DestroyExpParsor(void *ap_parsor);

	void *CreateExpParsorEx();
	const char *GetExpStringEx(void *ap_parsor);
	void InitExpParserEx(void *ap_parsor);
	void SetExpStringEx(void *ap_parsor, const char *ap_string);
	void RunExpParsingEx(void *ap_parsor);   // ����ڰ� ������ ������ �м��Ѵ�

	int GetExpVariableCountEx(void *ap_parsor);
	const char *GetExpVariableNameEx(void *ap_parsor, int a_index);
	// ������ ������ ������ ���� �ִ� ��ġ�� �ּҷ� �˷��ش�
	void SetExpVariablePointerEx(void *ap_parsor, int a_index, unsigned char a_data_type, double *ap_data);
	// �������� �ּҷ� �����ϵ��� �Ǿ� �ִ� ������ �ִٸ� �ش� �ּҿ��� ���� �о� �������� ������Ʈ �Ѵ�.
	void UpdateExpVariableValueEx(void *ap_parsor);
	// �̹� ������ parsing�Ǿ��ٸ� �ٽ� parsing�� �ʿ䰡 ���� ���Լ��� ȣ������, ExecExp �Լ��� ȣ���ϸ� �ȴ�
	char RestoreExpParserInfoEx(void *ap_parsor);
	char GetExpReadyFlagEx(void *ap_parsor);   // Parsing �� �Ǿ��ִ����� �˷��ش� ( 0 -> �Ľ̾ȵ�, 1 -> �Ľ̵� )
	ExpDataType ExecExpEx(void *ap_parsor, unsigned int a_prev_depth = 0);  // Parsing�� ������ �����Ͽ� ������ ������� ��´�
	void DestroyExpParsorEx(void *ap_parsor);

	// [ ��ũ�� ���� �Լ��� ]
	// �ý��� Ű�� �Է��ϴ� �Լ�. 
	void SystemKeyWrite(char a_key, int a_delay = 250);  // (Key Down & Up�� �� ���� ó��)
	void SystemKeyDown(char a_key);
	void SystemKeyUp(char a_key);
	// �Ϲ� Ű�� �Է��ϴ� �Լ�. Shift Ű�� ����ϴ� ��쿡�� a_is_shift�� 1�� �Է��ؾ� �Ѵ�. 
	void InputNormalChar(char a_is_shift, char a_key);  // (Key Down & Up�� �� ���� ó��)
	void InputNormalCharDown(char a_is_shift, char a_key);
	void InputNormalCharUp(char a_is_shift, char a_key);
	// ap_string�� ���޵� Ű����� �Է��Ѵ�.
	char InputNormalString(const char *ap_string, char a_is_han_mode, int a_delay = 100);
	void WaitInputSystem(int a_delay);

	void MouseMoveWrite(int a_x, int a_y);  // ���콺 �̵�!
	void MouseClickWrite(int a_x, int a_y, int a_is_left = 1, int a_delay = 0);
	void MouseClickDown(int a_x, int a_y, int a_is_left = 1);
	void MouseClickUp(int a_x, int a_y, int a_is_left = 1);

	void GetMousePosFromTarget(TargetData *ap_target, int *ap_x, int *ap_y, int a_target_x, int a_target_y);

	void *MakeTargetBaseData(int a_x, int a_y, int a_cx, int a_cy);  // ���� ĸ�� �̹��� ������ �����ϴ� �Լ�
	void DeleteTargetBaseData();

	void *GetTargetBaseData();
	UINT32 GetTargetBaseImageDataSize();
	UINT8 *GetTargetBaseImageData();

	void UpdateTargetImage();
	void UpdateTargetPos(TargetData *ap_target);
	// a_is_area_type (0: Client Area, 1: Window Area, 2: Screen Area) - 32Bit Color Only
	unsigned int GetTargetColor(TargetData *ap_target, int a_x, int a_y, int a_is_area_type = 0);
	TargetData *FindTargetImage(int a_index, const char *ap_class_name, const char *ap_title_name);
	TargetData *FindTargetImage(int a_index, int a_x, int a_y);
	void ShowTargetImage(TargetData *ap_target, int a_x, int a_y, int a_is_area_type);
	void ShowTargetImageEx(TargetData *ap_target, int a_x, int a_y, int a_cx, int a_cy, int a_is_area_type, int a_src_x, int a_src_y);
	void GetTargetImageData(unsigned int *ap_image_data, TargetData *ap_target, int a_is_area_type);
	void GetTargetImageDataEx(unsigned int *ap_image_data, TargetData *ap_target, int a_is_area_type, int a_x, int a_y, int a_cx, int a_cy);
	// ������ ��ǥ(a_x, a_y)�� �ִ� ����(a_org_color)�� �������� ���� ������ üũ�Ͽ� ����� ������ ����Ѵ�.
	// üũ�� �̹����� ap_image_data�̰� ã�� ������ ��ǥ�� p_target_rect�� �簢�� ��ǥ�� ����ȴ�.
	// ap_image_data�� ����� �̹��� ������ ���� a_image_width�̰� ���̴� a_image_height�̴�.
	void CheckOBjectPos(RECT *p_target_rect, int a_x, int a_y, unsigned int a_org_color, unsigned int a_change_color, unsigned int *ap_image_data, int a_image_width, int a_image_height);

	// �̹����� ī��â���� �����ϴ� �Լ�
	void SendImageToTalk(int a_cx, int a_cy, UINT8 *ap_image, int a_image_size, HWND ah_send_wnd);
	// �̹��� ������ Ȯ���ϴ� â�� üũ�ؼ� ���� ��ư�� �����ִ� �Լ�
	HWND CheckSendImageDlg();

	// Ŭ������ ���� �Լ�
	int CopyTextToClipboard(const char *ap_string);
	int CopyTextFromClipboard(char **ap_string, char a_is_clear);
	HBITMAP MakeDupBitmap(HBITMAP ah_bitmap);
	HBITMAP MakeDupBitmap(HBITMAP ah_bitmap, void *ap_image_data);
	HBITMAP MakeDupBitmap(int a_cx, int a_cy, int a_bits, void *ap_image_data, int a_data_size);

	HBITMAP CopyBitmapFromClipBoard();
	void CopyBitmapToClipBoard(HBITMAP ah_bitmap);
	void CopyBitmapToClipBoard(int a_cx, int a_cy, int a_bits, void *ap_image_data, int a_data_size);

	// Common Dialog ���� �Լ�
	int ChooseOpenFileName(char *ap_open_path, int a_path_size, const char *ap_filter = "All\0*.*\0Text\0*.txt\0", int a_filter_index = 1, const char *ap_init_path = NULL, int a_add_attr = 0);
	int ChooseSaveFileName(char *ap_open_path, int a_path_size, const char *ap_filter = "All\0*.*\0Text\0*.txt\0", int a_filter_index = 1, const char *ap_init_path = NULL, int a_add_attr = 0);
	int ChooseColorDlg(COLORREF a_init_color, COLORREF *ap_selected_color);
	int ChooseFontDlg(char *ap_font_name, int *ap_font_size, int *ap_font_style, COLORREF a_init_color = RGB(0, 0, 0), COLORREF *ap_selected_color = NULL);

	// �ø��� ��� ���� �Լ�
	void *CreateSerialObject();
	int Serial_OpenPort(void *ap_object, int a_port_number, HWND ah_notify_wnd, void(*ap_modify_dcb)(DCB *ap_dcb) = NULL, int a_notify_msg_id = 21000);
	int Serial_IsOpenPort(void *ap_object);  // ��Ʈ�� ���µǾ� ���� ������ 0, ���µǾ��ִٸ� ��Ʈ ��ȣ�� ��ȯ��. COM1�̸� 1�� ��ȯ!
	int Serial_GetRecvDataSize(void *ap_object);
	char *Serial_GetRecvDataBuffer(void *ap_object);
	void Serial_RemoveDataFromBuffer(void *ap_object, int a_remove_size);
	void Serial_ResumeWatchThread(void *ap_object);
	void Serial_SetRecvTimeoutInfo(void *ap_object, int a_count = 5, int a_interval = 300);
	int Serial_WriteCommData(void *ap_object, const char *ap_data, int a_length, int(*ap_error_process)(DWORD, COMSTAT) = NULL);
	void Serial_ClearEvent(void *ap_object, int a_tx = 1, int a_rx = 1);
	void DestroySerialObject(void *ap_object);

	// �ý��ۿ� ��ϵ� ��ġ ����� ��� �Լ�
	int GetRegisteredPrinterList(char a_printer_list[][512], int *ap_def_index);
	// ������ ��ġ�� ����̹��� �̸��� ��� �Լ�
	int GetNameFromPrinterInfo(char *ap_driver_name, char *ap_device_name, char *ap_printer_info);
	// �����Ϳ� ������ ����ϱ� ���� DC�� ��� �Լ�
	// ��� ��带 �����Ѵ�. 0.1 mm����, ���� ����(x �� , y ��), A4 ������ ũ��: 210mm x 297mm (2100 x -2970)
	HDC CreatePrintDC(char *ap_driver_name, char *ap_device_name);
	// ����� ��Ʈ�� ���� �����ϴ� �Լ�
	HFONT CreateDirectFont(const char *ap_name, int a_size, int a_bold, int a_italic, int a_underline, int a_strikeout);
	// ���� ���Ͽ��� ��Ʈ�� �и����ִ� �Լ�
	int GetExcelSheetFromZip(const char *ap_excel_file_name);

	UINT32 GetFIleDataToMemory(UINT8 **ap_data, const char *ap_file_path);
	UINT8 IsNormalUnicodeChar(const wchar_t a_char);
	// �����ڵ� ���ڿ��� �Ϲ� �ƽ�Ű ���ڿ��� �����ϴ� �Լ�
	int UnicodeToAscii(char **ap_dest_str, const wchar_t *ap_src_str);
	int UnicodeToSimpleAscii(char *ap_dest_str, const wchar_t *ap_src_str);

	// �����̷��� ���� �Լ�
	void *CreateRedirectionData();  // �����̷����� ����ϱ� ���� �ʿ��� �����͸� ����� �Լ�
	void DestroyRedirectionData(void *ap_data); // �����̷����� ����ϱ� ���� ���� �����͸� �����ϴ� �Լ�
	void StopRedirection(void *ap_data); // ���� ���� �����̷��� �۾��� ���� ��Ű�� �Լ�
	int IsExistPath(const char *ap_path);  // ������ ��ο� �ִ� ���� �Ǵ� ������ �ִ��� üũ�ϴ� �Լ�
	int GetRedirectionData(void *ap_data, char *ap_str, int a_limit);  // �������� �����̷��� ������ ���¸� ��� �Լ�
	int CheckRedirectionState(void *ap_data);  // �����̷��� �۾��� �ߴܵǾ����� üũ�ϴ� �Լ�

	// �����̷��ǰ� ���õ� Ÿ�̸� ������ �ٽ� �����ϴ� �Լ�
	void ModifyRedirectionTimerInfo(void *ap_data, UINT32 a_interval = 300, UINT32 a_timer_id = 77);
	// ������ ��ο� �ִ� ���α׷��� ����ؼ� �����̷����� �����ϴ� �Լ�
	int ExecRedirection(void *ap_data, const char *ap_exe_path, const char *ap_arg_data, 
			const char *ap_working_path, void *afp_timer_proc);
}

#define MAX_TABLE_COUNT          65536  // ���� ���� ���� ��
#define COMMON_RECV_SIZE          8192  // �⺻ ���� ���� (8K)
#define COMMON_SEND_SIZE          8192  // �⺻ �۽� ���� (8K)
#define USER_RECV_SIZE          204800  // �⺻ ���� ���� (200K)
#define USER_SEND_SIZE          204800  // �⺻ �۽� ���� (200K)
#define TSB_SIZE                  4096  // �� ���̺귯�� ������ �ӽ÷� ����� ������ ũ��

#define SERVER_SOCKET_ACCEPT     21901  // ������ Ŭ���̾�Ʈ�� ������ �� �߻��ϴ� �޽���
#define SERVER_SOCKET_CLIENT     21902  // ������ ������ Ŭ���̾�Ʈ�� ��� �Ǵ� ������ �� �߻��ϴ� �޽���
#define SERVER_SOCKET_USER       21903  // ������ �α����� ����ڿ� ��� �Ǵ� ������ �� �߻��ϴ� �޽���

#define CLIENT_SOCKET_CONNECT    21911  // Ŭ���̾�Ʈ�� ������ ������ �õ��� ����� �˷��ִ� �޽���
#define CLIENT_SOCKET_COMMON     21912  // ������ ��� �Ǵ� ������ ������ �� �߻��ϴ� �޽���

#define SERVER_STATE_NOTIFY      21950  // ���� ������ ���¸� �����ϴ� �޽���
#define CLIENT_STATE_NOTIFY      21951  // Ŭ���̾�Ʈ ������ ���¸� �����ϴ� �޽���

// [��Ʈ��ũ ���ξ� ��Ģ]  Client -> Server (Request) : REQ_,  Server -> Client (Answer) : ANS_
#define ANS_WELCOME_MSG             0    // S -> C ���� ��� �޽���
#define ANS_ALIVE_MSG               1    // S -> C ���� ���� �޽���
#define REQ_MAKE_ID                 6    // C -> S ���� ����� ��û�ϴ� �޽���
#define ANS_MAKED_ID                7    // S -> C ���� ����� ������
#define REQ_LOGIN_DATA             10    // C -> S �α��� �õ�
#define ANS_LOGIN_OK               11    // S -> C �α��� ����
#define ANS_LOGIN_FAIL             12    // S -> C �α��� ����
#define ANS_SYSTEM_MESSAGE         13    // S -> C �������� ���޵� �ý��� ���� �޽���   

#define LEN_TO_SIZE(x)                (x+1) // ���ڿ� ���̸� ���ڿ��� �����ϴµ� �ʿ��� ũ��� ����
#define SIZE_TO_LEN(x)                (x-1) // ���ڿ��� �����ϴµ� �ʿ��� ũ�⸦ ���ڿ� ���̷� ����
#define PushStrToStream(s, str, size) *s++ = size; memcpy(s, str, size); s += size

struct NeoClientData  // 1���� Ŭ���̾�Ʈ ������ ������ ����ü
{
	UINT8 msg_id;                // ���� ó������ ��Ʈ��ũ �޽��� ID
	UINT32 data_size, cur_size;  // ��ü ������ ũ��� ���� ���ŵ� ũ��
	UINT8 *p_recv_data;          // ���ŵ� �������� ���� ����
	SOCKET h_socket;             // ���� Ŭ���̾�Ʈ�� ����� ���� �ڵ�
	char ip_address[16];         // ������ Ŭ���̾�Ʈ�� �ּ�
	UINT32 connect_time;         // 30�� �̳��� �α����� �ϴ��� üũ
};

struct LoginData
{
	char id[32];                 // ���̵�
	char password[32];           // ��ȣ
};

typedef struct NeoUserData
{
	UINT8 level;  // (����) 0:�մ�, 1:�Ϲ�, 2:���, 3:���, 4:������
	UINT8 id_len, password_len, name_len, etc_len;  // ���ڿ� �濡�� NULL ���� ����
	char id[32];                // ����� ���̵�
	char password[32];          // ��ȣ
	char name[32];              // �̸�
	char etc[128];              // ��Ÿ ����
	UINT32 last_connected_time; // ���������� ������ �ð� ����
	void *p_ext;                // �߰� ������ �ʿ��� ��� ���
} NUD;

typedef struct NeoRuntimeUserData
{
	NUD info;  // ����� �⺻ ����

	UINT8 is_try_pw_count;       // �α��� �õ� Ƚ��
	UINT8 is_block;              // �α��� �õ��� 10�� �����ϸ� ���� ��� ����!
	UINT8 ip_len;                // ip �ּ��� ����
	UINT8 key;                   // ��ſ� ����� Ű
	UINT8 msg_id;                // ���� ó������ ��Ʈ��ũ �޽��� ID
	UINT32 data_size, cur_size;  // ��ü ������ ũ��� ���� ���ŵ� ũ��
	UINT32 last_use_time;        // ���� �������� ������ ����� �ð� (�۽�, ���� ����)
	UINT8 *p_recv_data;          // ���ŵ� �������� ���� ����
	char ip[16];                 // ������ ������� IP �ּ�
	SOCKET h_socket;             // ������ Ŭ���̾�Ʈ�� ���� �ڵ�
} NRUD;

struct NeoServerData;
typedef void (*FP_EVENT_STR)(NeoServerData *ap_server, const char *ap_string);  // �̺�Ʈ�� �߰��ϱ� ���� �Լ�
typedef int (*FP_USER_MSG)(NeoServerData *ap_server, NRUD *ap_user); // ����ڿ� ����ϱ� ���� ������ �Լ�
typedef void (*FP_USER_NOTIFY)(NeoServerData *ap_server, NRUD *ap_user, UINT32 a_state); // ����� ���� �뺸�� �Լ�

struct NeoServerData
{
	SOCKET h_listen_socket;            // Ŭ���̾�Ʈ�� ������ ó���ϱ� ���� ���� �ڵ�
	UINT8 *p_send_data, *p_recv_data;  // Ŭ���̾�Ʈ�� �����͸� �����ϰų� ������ �� ����� �޸�
	char *p_buffer1, *p_buffer2;       // TSB_SIZE ũ��� �Ҵ�
	UINT8 base_key;                    // �������ݿ� ����� �⺻ Ű
	UINT16 client_count;               // ���� �õ��� ����� �⺻ Ŭ���̾�Ʈ ��
	UINT16 user_count;                 // ������ ���� ����� ��
	NeoClientData *p_clients;          // ������ Ŭ���̾�Ʈ ����
	NRUD *p_users;                     // ��ϵ� ����� ����
	NRUD *p_last_user;                 // ����� �������� ���� �������� ��ġ�� ����� ��ġ�� ����� �޸��� �ּ�

	void *p_table[MAX_TABLE_COUNT];    // �� ���̺귯���� ����ϴ� ���� ���̺�
	FP_USER_MSG p_user_msg_func;       // ����ڰ� ������ �޽����� ó���� �Լ��� �ּ�
	FP_EVENT_STR p_event_func;         // �̺�Ʈ �α׸� ó���� �Լ��� �ּ�
	FP_USER_NOTIFY p_user_notify_func; // ����� ���¸� �뺸�Ҷ� ����� �Լ��� �ּ�
};

namespace TW_NeoServerSocket
{
	// ���� ������ ����� ��ǻ�Ϳ��� ��� ������ IP ����� ��� �Լ�
	int GetLocalNetworkAddress(char ap_ip_list[][16]);
	// ���� ������ ������ �ʱ�ȭ�Ѵ�.
	void InitServerData(NeoServerData *ap_server, FP_USER_MSG ap_user_msg_func, FP_EVENT_STR ap_event_func, FP_USER_NOTIFY ap_user_notify_func, int a_max_client_count = 50, int a_max_user_count = 100, int a_key = 0x29);
	// ���� ���񽺸� �����ϴ� �Լ�
	void StartListenService(const char *ap_ip_address, int a_port, NeoServerData *ap_server);
	// ���� ���񽺸� ���� �����ߴ� ������ �����ϴ� �Լ�
	void CleanUpServerData(NeoServerData *ap_server);
	// ������� ���� ���ϰ� ���õ� ������ ��� �����ϴ� �Լ�
	void DestroyServerSocket(NeoServerData *ap_server);
	// ��� ���� ������ �ݴ� �Լ� (closesocket���� ��ü ����)
	void CloseSocketNoWait(SOCKET ah_socket);

	// Ŭ���̾�Ʈ ������ ������ �� ������ ã�� �Լ�
	NeoClientData *CheckEmptyCommonSlot(NeoServerData *ap_server);
	// Ŭ���̾�Ʈ�� �����͸� �����ϴ� �Լ�, (6����Ʈ ������ ���� : "[ Key(1), Message ID(1), Body size(4)] +  Body Data")
	void SendCommonFrameData(NeoServerData *ap_server, NeoClientData *ap_client, UINT8 a_msg_id, const void *ap_data, UINT32 a_size);
	// Ŭ���̾�Ʈ ���� �õ��� ó���ϴ� �Լ�
	void ProcessCommonAccept(NeoServerData *ap_server);
	// Ư�� Ŭ���̾�Ʈ�� �������� ������ ������ �� ����ϴ� �Լ�
	void DisconnectCommonClientByError(NeoServerData *ap_server, NeoClientData *ap_client, const char *ap_error_msg);
	// ���ŵ� �����Ͱ� �������� ���Ŀ� �´��� �׸��� ������ ���ŵǾ����� �����ϴ� �Լ�
	int ProcessingCommonMessage(NeoServerData *ap_server, NeoClientData *ap_client, UINT8 *ap_recv_data, int a_recv_size);
	// ���Ͽ� �߻��ϴ� �̺�Ʈ�� ó���ϴ� �Լ�
	void ProcessCommonSocketEvent(NeoServerData *ap_server, SOCKET ah_socket, int a_event_type);
	// ���̵� ������ ���ڿ��� ��ȿ���� �˻��ϴ� �Լ�
	int CheckStringThatMakesUpID(const char *ap_id_str);
	// �ߺ��� ID�� ���� ����ڸ� ã�� �Լ�
	NRUD *GetDupUserByID(NeoServerData *ap_server, NRUD *ap_cur_user, char *ap_id, int a_len);
	// ����� �������� ���� ������ ���� ������ ã�� �Լ�
	NRUD *GetEmptySlot(NeoServerData *ap_server);


	//// �α��ε� ����ڸ� ���� �Լ� ////

	// ����� ��Ͽ��� ������ ��ϵ� ������ ������� �ּҸ� �����ϴ� �Լ�
	void SetLastUser(NeoServerData *ap_server, NRUD *ap_last_users);
	// ���ڿ��� ���̸� ���ϴ� �Լ�. ������ �� ���ڿ��� ���̴� a_max_len���� Ŭ�� ����.
	int GetStringLength(const char *ap_str, int a_max_len);
	// �α��� ������ ��ϵ� ����ڿ� ���ϴ� �Լ�
	NRUD *CheckLoginData(NeoServerData *ap_server, LoginData *ap_data);
	// ���ӵ� ����ڸ� ������ ���� ������Ų��.
	void CloseUserSocket(NeoServerData *ap_server, NRUD *ap_user, char a_reset_flag);
	// Ŭ���̾�Ʈ�� �����͸� �����ϴ� �Լ�, (6����Ʈ ������ ���� : "[ Key(1), Message ID(1), Body size(4)] +  Body Data")
	void SendUserFrameData(NeoServerData *ap_server, NRUD *ap_user, UINT8 a_msg_id, const void *ap_data, UINT32 a_size);
	// ������ ��ü ����ڿ��� �޽����� ������ �Լ�
	void SendBroadcastUserFrameData(NeoServerData *ap_server, UINT8 a_msg_id, const void *ap_data, UINT32 a_size);
	// ������ ����ڵ鿡�� �޽����� ������ �Լ�
	void SendBroadcastUserFrameDataEx(NeoServerData *ap_server, NRUD **ap_user, int a_count, UINT8 a_msg_id, const void *ap_data, UINT32 a_size);
	// ����� ���Ͽ� �߻��� �̺�Ʈ�� ó���ϴ� �Լ�
	void ProcessUserSocketEvent(NeoServerData *ap_server, SOCKET ah_socket, int a_event_type);
	// ���ŵ� �����Ͱ� �������� ���Ŀ� �´��� �׸��� ������ ���ŵǾ����� �����ϴ� �Լ�
	int ProcessingUserMessage(NeoServerData *ap_server, NRUD *ap_user, UINT8 *ap_recv_data, int a_recv_size);
	// Ư�� ����� ������ ������ ������ �� ����ϴ� �Լ�
	void DisconnectUserByError(NeoServerData *ap_server, NRUD *ap_user, const char *ap_error_msg, char a_reset_flag);
}


// Ŭ���̾�Ʈ ���� ��Ʈ

struct NeoSocketData;
typedef void (*FP_EVENT_STR_CLIENT)(NeoSocketData *ap_client, const char *ap_string);  // �̺�Ʈ�� �߰��ϱ� ���� �Լ�
typedef int (*FP_USER_MSG_CLIENT)(NeoSocketData *ap_client); // ������ ����ϱ� ���� ������ �Լ�
typedef void (*FP_CLIENT_NOTIFY)(NeoSocketData *ap_client, UINT32 a_state); // Ŭ���̾�Ʈ ���� �뺸�� �Լ�

struct NeoSocketData
{
	UINT8 msg_id, is_connected;  // ���� ó������ ��Ʈ��ũ �޽��� ID, �������� ���� ����(0:���Ӿȵ�, 1:������, 2:�������, 3: �α��� ����)
	UINT8 base_key;  // ��ſ� ����� �⺻ Ű��
	UINT32 data_size, cur_size;  // ��ü ������ ũ��� ���� ���ŵ� ũ��
	UINT8 *p_recv_data;          // ���ŵ� �������� ���� ����
	UINT8 *p_send_data, *p_temp_recv_data; // ������ �����͸� �����ϰų� ������ �� ����� �޸�
	SOCKET h_socket;  // ������ ����� �ϱ� ���� ���� �ڵ�
	LoginData login_info;  // ������ �α��ο� ����� ���� ����

	FP_USER_MSG_CLIENT p_user_msg_func; // ������ ������ �޽����� ó���� �Լ��� �ּ�
	FP_EVENT_STR_CLIENT p_event_func;   // �̺�Ʈ �α׸� ó���� �Լ��� �ּ�
	FP_CLIENT_NOTIFY p_notify_func;    // Ŭ���̾�Ʈ ���� �뺸�� �Լ��� �ּ�
};

namespace TW_NeoClientSocket
{
	void InitData(NeoSocketData *ap_data, FP_USER_MSG_CLIENT ap_user_msg_func, FP_EVENT_STR_CLIENT ap_event_func, FP_CLIENT_NOTIFY ap_notify_func, int a_key = 0x29);  // Ŭ���̾�Ʈ ������ ������ �ʱ�ȭ�Ѵ�.
	void DestroySocket(NeoSocketData *ap_data);  // ������� Ŭ���̾�Ʈ ������ �����ϴ� �Լ�
	void CleanUpClientData(NeoSocketData *ap_data);  // Ŭ���̾�Ʈ ���񽺸� ���� �����ߴ� ������ �����ϴ� �Լ�
	void ConnectToServer(NeoSocketData *ap_data, const char *ap_ip_address, int a_port); // ������ ������ �õ��ϴ� �Լ�
	// ������ �����͸� �����ϴ� �Լ�, (6����Ʈ ������ ���� : "[ Key(1), Message ID(1), Body size(4)] +  Body Data")
	void SendFrameData(NeoSocketData *ap_socket_data, UINT8 a_msg_id, const void *ap_data, UINT32 a_size);
	void ProcessConnectionResult(NeoSocketData *ap_data, int a_error_code); // ���� ���� ����� ó���ϴ� �Լ�

	// ���ŵ� �����Ͱ� �������� ���Ŀ� �´��� �׸��� ������ ���ŵǾ����� �����ϴ� �Լ�
	int ProcessingMessage(NeoSocketData *ap_data, UINT8 *ap_recv_data, int a_recv_size);
	// ���Ͽ� �߻��ϴ� �̺�Ʈ�� ó���ϴ� �Լ�
	void ProcessSocketEvent(NeoSocketData *ap_data, int a_event_type);
}

extern const char *gp_app_name;
extern const char *gp_window_title;
extern HWND gh_main_wnd;
#endif