struct Cam_Group
{
	short cameragroupid;
	CString camera_group_desc;
	CString default_;
	CString category_code;
	CString module_code;
	CString map_file;
	CString acmap_file;

	Cam_Group()
		:cameragroupid(0),camera_group_desc(_T("")),default_(_T("")),category_code(_T("")),
		module_code(_T("")),map_file(_T("")),acmap_file(_T("")){}

	void clear()
	{
		cameragroupid=0;
		camera_group_desc = _T("");
		default_ = _T("");
		category_code = _T("");
		module_code = _T("");
		map_file = _T("");
		acmap_file = _T("");
	}

	bool isempty()
	{
		if (cameragroupid == 0 || camera_group_desc.IsEmpty())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

struct Cam_Group_Cam
{
	int cameragroupcameraid;
	short cameragroupid;
	short cameraid;
	short camera_idx;
	int streamid;
	short numb_;	//Check for device type. If a object is a device(like NVR), the value will be 1.

	int emap_x;
	int emap_y;
	int acmap_x;
	int acmap_y;

	Cam_Group_Cam()
		:cameragroupid(0),cameraid(0),camera_idx(0),numb_(0),
		emap_x(0),emap_y(0),acmap_x(0),acmap_y(0),cameragroupcameraid(0),streamid(0){}
	void clear()
	{
		cameragroupcameraid = 0;
		cameragroupid = 0;
		cameraid = 0;
		camera_idx = 0;
		numb_ = 0;
		emap_x = 0;
		emap_y = 0;
		acmap_x = 0;
		acmap_y = 0;
		streamid = 0;
	}

	bool isempty()
	{
		if (cameragroupid == 0 || cameraid == 0 || cameragroupcameraid == 0)
		{
			return true;
		} 
		else
		{
			return false;
		}
	}
};

struct ec_Stream
{
	CString stream_name;
	CString stream_tag;
	int cameraid;
	CString stream_type;
	short stream_port;
	CString stream_protocol;
	CString video_cmpr_type;
	CString video_res;

	ec_Stream()
		:stream_name(_T("")),stream_protocol(_T("")),stream_port(0),cameraid(0),stream_type(_T("")),
		stream_tag(_T("")),video_cmpr_type(_T("")),video_res(_T("")){}

	void clear()
	{
		stream_name = _T("");
		stream_protocol = _T("");
		stream_port = 0;
		cameraid = 0;
		stream_type = _T("");
		video_cmpr_type = _T("");
		video_res = _T("");
	}

};
struct ec_Camera
{
	short cameraid;
	short camera_idx;
	CString cameraname;
	CString connect_type_code; 
	CString ipaddress;
	short httpport;
	CString gateway;
	CString name_server;
	CString mac_address;
	CString username;
	CString password;
	CString brand_code;
	CString model_code;
	CString ptz_support;
	CString ptz_protocol;
	CString digital_in1;
	CString digital_in2;
	CString digital_out;
	CString video_format;
	CString speaker_support;
	CString mic_support;
	CString subnet_mask1;
	CString subnet_mask2;
	CString subnet_mask3;
	CString subnet_mask4;
	CString active_;
	CString stream_url;

	vector<ec_Stream> vcStream;

	ec_Camera()
		:stream_url(_T("")),cameraname(_T("")),ipaddress(_T("")),mac_address(_T("")),
		username(_T("")),password(_T("")),model_code(_T("")),video_format(_T("")),
		cameraid(0),camera_idx(0),httpport(0),vcStream(),ptz_support(_T("")),active_(_T("")),
		connect_type_code(_T("")),gateway(_T("")),name_server(_T("")),ptz_protocol(_T("")),
		digital_in1(_T("")),digital_in2(_T("")),digital_out(_T("")),subnet_mask1(_T("")),
		subnet_mask2(_T("")),subnet_mask3(_T("")),subnet_mask4(_T("")),brand_code(_T("")),
		speaker_support(_T("")),mic_support(_T("")){}

	void clear()
	{
		stream_url = _T("");
		cameraname = _T("");
		ipaddress = _T("");
		mac_address = _T("");
		username = _T("");
		password = _T("");
		model_code = _T("");
		video_format = _T("");
		cameraid = 0;
		camera_idx = 0;
		httpport = 0;
		vcStream.clear();

		ptz_support = _T("");
		active_ = _T("");
		connect_type_code = _T("");;
		gateway = _T("");
		name_server = _T("");
		ptz_protocol = _T("");
		digital_in1 = _T("");
		digital_in2 = _T("");
		digital_out = _T("");
		subnet_mask1 = _T("");
		subnet_mask2 = _T("");
		subnet_mask3 = _T("");
		subnet_mask4 = _T("");
		brand_code = _T("");
		speaker_support = _T("");
		mic_support = _T("");
	}

	bool isempty()
	{
		if (mac_address.GetLength())
			return false;
		else
			return true;
	}
};
struct ec_Camera_Rec
{
	int camera_idx;
	CString recording_type;
	CString recording_time;

	ec_Camera_Rec()
		:camera_idx(0),recording_type(_T("")),recording_time(_T(""))
	{}

	void clear()
	{
		recording_time = _T("");
		recording_type = _T("");
		camera_idx = 0;
	}

	bool isempty()
	{
		if (!camera_idx)
			return false;
		else
			return true;
	}
};
struct ec_Storage
{
	CString storage_type;
	CString store_location;
	CString buffer_size;
	CString server_user;
	CString server_password;

	ec_Storage()
		:storage_type(_T("")),store_location(_T("")),buffer_size(_T("")),
		server_user(_T("")),server_password(_T("")){}

	void clear()
	{
		storage_type = _T("");
		store_location = _T("");
		buffer_size = _T("");
		server_user = _T("");
		server_password = _T("");
	}
};