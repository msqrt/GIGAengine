
#include "main.h"
#include "resource1.h"
#include <commctrl.h>

#include <random>
#include <vector>
#include <array>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "ComCtl32.lib")
#pragma comment(lib, "msvcrt.lib")

#pragma comment(linker, "/nodefaultlib:libcmt.lib")

#ifdef _DEBUG
#pragma comment(lib, "strmbasd.lib")
#else
#pragma comment(lib, "strmbase.lib")
#endif

#pragma comment(lib, "winmm.lib")

bool runprogram = true, full = false;

int screenw = 1280, screenh = 720;

INT_PTR CALLBACK startupproc(HWND dlg, UINT msg, WPARAM w, LPARAM l)
{
	if (msg == WM_INITDIALOG)
	{
		ComboBox_AddString(GetDlgItem(dlg, IDC_COMBO1), L"1280x720");
		ComboBox_AddString(GetDlgItem(dlg, IDC_COMBO1), L"1920x1080");
		ComboBox_AddString(GetDlgItem(dlg, IDC_COMBO1), L"3840x2160");
		ComboBox_AddString(GetDlgItem(dlg, IDC_COMBO1), L"WxH (write your own)");

		ComboBox_SetCurSel(GetDlgItem(dlg, IDC_COMBO1), 0);

		Button_SetCheck(GetDlgItem(dlg, IDC_CHECK1), true);
	}
	if (msg == WM_COMMAND)
	{

		if (HIWORD(w) == BN_CLICKED && LOWORD(w) == IDCANCEL)
		{
			runprogram = false;
			EndDialog(dlg, 0);
			return 1;
		}

		if (HIWORD(w) == BN_CLICKED && LOWORD(w) == IDOK) // launch pressed
		{
			int len = 1 + ComboBox_GetTextLength(GetDlgItem(dlg, IDC_COMBO1));
			wchar_t * buff = new wchar_t[len];
			ComboBox_GetText(GetDlgItem(dlg, IDC_COMBO1), buff, len);

			swscanf_s(buff, L"%dx%d", &screenw, &screenh);

			delete[] buff;

			if (BST_CHECKED == IsDlgButtonChecked(dlg, IDC_CHECK1))
				full = true;

			EndDialog(dlg, 0);
			return 1;
		}
	}
	return 0;
}

#define BUILDING_AMOUNT 1//600
#define VORONOI_AMOUNT 30

//int CALLBACK main(HINSTANCE self, HINSTANCE prev, LPSTR cmdline, int show) {
int main() {

	InitCommonControls();

	//DialogBox(GetModuleHandle(0), MAKEINTRESOURCE(IDD_DIALOG1), 0, (DLGPROC)startupproc);

	if (!runprogram)
		ExitProcess(0);

	window win(screenw, screenh, full, L"PEISIK");
	//window win(1920, 1080, 1, L"windy window");

	auto set_mouse = [&]() {

		RECT r;
		GetWindowRect(win.windowHandle, &r);
		POINT p;

		GetCursorPos(&p);
		int x2 = r.left + (r.right - r.left) / 2,
			y2 = r.top + (r.bottom - r.top) / 2;
		SetCursorPos(x2, y2);
		return std::pair<int, int>(p.x - x2, p.y - y2);
	};

	ShowCursor(0);

	((PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT"))(1);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	unsigned loops = 0;
	GLuint query; GLint res;
	glGenQueries(1, &query);

	mesh quad(QUAD);

	song track(L"musa.mp3");

	double dirx = .0, diry = .0;
	pos camera;
	POINT pt = { win.width / 2, win.height / 2 };

	float t = .0f;

	bool flymode = true;
	if (flymode) {
		win.mousex = win.width / 2;
		win.mousey = win.height / 2;
		set_mouse();
	}
	struct vertex {
		pos position;
		nor normal;
	};

	vertex box_corners[6 * 6];

	// generate quads
	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 4; ++j) {
			box_corners[i * 4 + j].normal[i / 2] = (i % 2) ? -1.0f : 1.0f;
			vec a(.0f); a[((i / 2) + 1) % 3] = (i % 2) ? 1.0f : -1.0f;
			vec b = cross(a, vec(box_corners[i * 4 + j].normal));
			a = (j < 2) ? a : -a;
			b = (j % 2) ? b : -b;
			box_corners[i * 4 + j].position = pos(vec(box_corners[i * 4 + j].normal) + a + b);
		}
	}

	// switch to tris
	for (int i = 6 - 1; i >= 0; --i) {
		vertex
			a = box_corners[i * 4 + 0],
			b = box_corners[i * 4 + 1],
			c = box_corners[i * 4 + 2],
			d = box_corners[i * 4 + 3];
		box_corners[i * 6 + 0] = a;
		box_corners[i * 6 + 1] = b;
		box_corners[i * 6 + 2] = c;
		box_corners[i * 6 + 3] = b;
		box_corners[i * 6 + 4] = d;
		box_corners[i * 6 + 5] = c;
	}

	shader default("box.shader");

	using namespace std;

	vector<vertex> city_verts;

	default_random_engine qwer(0);
	auto rand = [&](float a, float b)->float{ return uniform_real_distribution<float>(a, b)(qwer); };
	auto rand_int = [&](int a, int b)->int{ return uniform_int_distribution<int>(a, b)(qwer); };
	auto rand_exp = [&](float l)->float{ return exponential_distribution<float>(l)(qwer); };

	struct poisson_disk {
		float x, y, r;
	};
	vector<poisson_disk> city_disks;

	for (int j = 0; j < 6 * 6; ++j) {
		vertex b = box_corners[j];
		if (b.position.y < .4f) {
			b.position.y = -.1f;
			b.position.x *= 80.0f;
			b.position.z *= 80.0f;
		}
		else {
			b.position.y = .0f;
			b.position.x *= 80.0f;
			b.position.z *= 80.0f;
		}
		city_verts.push_back(b);
	}

	//cout << "breaking things... " << endl;

	for (int i = 0; i < BUILDING_AMOUNT; ++i) {
		float x, z;
		bool overlap;
		float cur_scale;
		while (.8f>(cur_scale = rand_exp(.5f)) || 4.0 < cur_scale);
		do {
			x = rand(-75.0f, 75.0f);
			z = rand(-75.0f, 75.0f);
			overlap = false;
			for (auto& a : city_disks) {
				if (sqrt((a.x - x)*(a.x - x) + (a.y - z)*(a.y - z)) < a.r + cur_scale) {
					overlap = true;
					break;
				}
			}
		} while (overlap);
		poisson_disk curdisk;
		curdisk.x = x; curdisk.y = z; curdisk.r = cur_scale;
		city_disks.push_back(curdisk);
		trf model_to_world = trf::translate(pos(x, .0f, z)) * trf::rotate(rand(-.1f, .1f), vec(.0f, 1.0f, .0f));
		int floors = rand_int(1, rand_int(1, 9));
		float cur_height = .0f;


		vector<vertex> building_verts;

		pos max_pos(-1000.0f), min_pos(1000.0f);

		for (int k = 0; k < floors; ++k) {
			float next_scale = cur_scale*rand(.6f, 1.2f);
			float floor_height = rand_exp(cur_scale*1.0f);

			for (int j = 0; j < 6 * 6; ++j) {
				vertex b = box_corners[j];
				if (abs(b.normal.y) < .01f || (b.normal.y > .0f && k == floors - 1) || (b.normal.y<.0f && k==0)) {
					if (b.position.y < .4f) {
						b.position.y = cur_height;
						b.position.x *= cur_scale;
						b.position.z *= cur_scale;
					} else {
						b.position.y = cur_height + floor_height;
						b.position.x *= next_scale;
						b.position.z *= next_scale;
					}
					max_pos = pos(max(max_pos.x, b.position.x), max(max_pos.y, b.position.y), max(max_pos.z, b.position.z));
					min_pos = pos(min(min_pos.x, b.position.x), min(min_pos.y, b.position.y), min(min_pos.z, b.position.z));
					building_verts.push_back(b);
				}
			}
			cur_height += floor_height;
			cur_scale = next_scale;
		}

		vector<pos> voronoi_points(VORONOI_AMOUNT);
		for (auto& p : voronoi_points)
			p = pos(rand(min_pos.x, max_pos.x), rand(min_pos.y, max_pos.y), rand(min_pos.z, max_pos.z));

		struct plane {
			nor normal;
			float d;
			bool keep;
			plane(nor n, float d) : normal(n), d(d), keep(true) {}
			plane(nor n, pos d) : normal(n), d(-dot(vec(d), vec(normal))), keep(true) {}
			pos intersect(pos p1, pos p2) {
				float t = -eval(p1) / dot(nor(p2 - p1), normal);
				return p1 + t*(p2 - p1);
			}
			float eval(pos position) {
				return dot(vec(position), vec(normal)) + d;
			}
		};

		vector<vertex> building_tmp = building_verts;
		building_verts.clear();

		//for (auto& p : voronoi_points)
		{
			auto& p = voronoi_points[0];
			vector<pos> tmp_pts = voronoi_points;
			vector<std::pair<plane, pos>> planes; planes.reserve(voronoi_points.size());
			
			for (auto& t : tmp_pts)
				if ((t - p).sqlen()>.01f)
					planes.push_back(std::make_pair(plane(nor(t - p), t), t));
			/*
			for (int j = 0; j < planes.size(); ++j) {
				for (int i = 0; i < planes.size(); ++i) {
					if (j == i)
						continue;

					if (planes[j].first.eval(planes[i].second) > .0f) {
						planes[i].first.keep = false;
					}
				}
			}
			auto iter = planes.begin();
			while (iter != planes.end()) {
				if (!iter->first.keep)
					iter = planes.erase(iter);
				else
					++iter;
			}
			*/
			for (auto& pl : planes) {
				pl.first.d = -dot(pl.first.normal, nor(.5f*(p+pl.second)));
			}
			vector<vertex> cut_mesh = building_tmp;
			for (auto& pl : planes) {
				vector<vertex> next_iter; next_iter.reserve(1000);
				vector<std::pair<pos,pos>> border; border.reserve(1000);
				for (int i = 0; i < cut_mesh.size(); i += 3) { // tri by tri
					if (pl.first.eval(cut_mesh[i + 0].position) < .0f &&
						pl.first.eval(cut_mesh[i + 1].position) < .0f &&
						pl.first.eval(cut_mesh[i + 2].position) < .0f) {

						next_iter.push_back(cut_mesh[i + 0]); // keep this, it's good
						next_iter.push_back(cut_mesh[i + 1]);
						next_iter.push_back(cut_mesh[i + 2]);
					}
					else if (
						pl.first.eval(cut_mesh[i + 0].position) < .0f &&
						pl.first.eval(cut_mesh[i + 1].position) < .0f) {

						next_iter.push_back(cut_mesh[i + 0]);
						next_iter.push_back(cut_mesh[i + 1]);
						vertex new_one, new_two;
						new_one.position = pl.first.intersect(cut_mesh[i + 0].position, cut_mesh[i + 2].position);
						new_two.position = pl.first.intersect(cut_mesh[i + 1].position, cut_mesh[i + 2].position);
						next_iter.push_back(new_two);

						next_iter.push_back(cut_mesh[i + 0]);
						next_iter.push_back(new_two);
						next_iter.push_back(new_one);
						border.push_back(std::make_pair(new_one.position, new_two.position));
					}
					else if (
						pl.first.eval(cut_mesh[i + 1].position) < .0f &&
						pl.first.eval(cut_mesh[i + 2].position) < .0f) {

						next_iter.push_back(cut_mesh[i + 1]);
						next_iter.push_back(cut_mesh[i + 2]);
						vertex new_one, new_two;
						new_one.position = pl.first.intersect(cut_mesh[i + 1].position, cut_mesh[i + 0].position);
						new_two.position = pl.first.intersect(cut_mesh[i + 2].position, cut_mesh[i + 0].position);
						next_iter.push_back(new_two);

						next_iter.push_back(cut_mesh[i + 1]);
						next_iter.push_back(new_two);
						next_iter.push_back(new_one);
						border.push_back(std::make_pair(new_one.position, new_two.position));
					}
					else if (
						pl.first.eval(cut_mesh[i + 2].position) < .0f &&
						pl.first.eval(cut_mesh[i + 0].position) < .0f) {

						next_iter.push_back(cut_mesh[i + 2]);
						next_iter.push_back(cut_mesh[i + 0]);
						vertex new_one, new_two;
						new_one.position = pl.first.intersect(cut_mesh[i + 2].position, cut_mesh[i + 1].position);
						new_two.position = pl.first.intersect(cut_mesh[i + 0].position, cut_mesh[i + 1].position);
						next_iter.push_back(new_two);

						next_iter.push_back(cut_mesh[i + 2]);
						next_iter.push_back(new_two);
						next_iter.push_back(new_one);
						border.push_back(std::make_pair(new_one.position, new_two.position));
					}
					else if (pl.first.eval(cut_mesh[i + 0].position) < .0f) {
						vertex new_one, new_two;
						new_one.position = pl.first.intersect(cut_mesh[i + 1].position, cut_mesh[i + 0].position);
						new_two.position = pl.first.intersect(cut_mesh[i + 2].position, cut_mesh[i + 0].position);
						
						next_iter.push_back(cut_mesh[i + 0]);
						next_iter.push_back(new_one);
						next_iter.push_back(new_two);
						border.push_back(std::make_pair(new_two.position, new_one.position));
					}
					else if (pl.first.eval(cut_mesh[i + 1].position) < .0f) {
						vertex new_one, new_two;
						new_one.position = pl.first.intersect(cut_mesh[i + 2].position, cut_mesh[i + 1].position);
						new_two.position = pl.first.intersect(cut_mesh[i + 0].position, cut_mesh[i + 1].position);

						next_iter.push_back(cut_mesh[i + 1]);
						next_iter.push_back(new_one);
						next_iter.push_back(new_two);
						border.push_back(std::make_pair(new_two.position, new_one.position));
					}
					else if (pl.first.eval(cut_mesh[i + 2].position) < .0f) {
						vertex new_one, new_two;
						new_one.position = pl.first.intersect(cut_mesh[i + 0].position, cut_mesh[i + 2].position);
						new_two.position = pl.first.intersect(cut_mesh[i + 1].position, cut_mesh[i + 2].position);

						next_iter.push_back(cut_mesh[i + 2]);
						next_iter.push_back(new_one);
						next_iter.push_back(new_two);
						border.push_back(std::make_pair(new_two.position, new_one.position));
					}
				}
				
				//for (auto& b : border)
				//	b = make_pair(b.second, b.first);

				for (int i = 0; i < int(border.size()) - 1; ++i) {
					int minel = i + 1;
					float mindist = 100.0f;
					for (int j = i + 1; j < border.size(); ++j) {
						float l = (border[i].second - border[j].first).sqlen();
						if (l < mindist) {
							mindist = l;
							minel = j;
						}
					}
					if (minel != i + 1) {
						swap(border[i + 1], border[minel]);
					}
				}

				while (border.size() > 2) {
					auto iter = border.begin();
					bool changes = false;
					while (iter != border.end()) {
						auto next = iter + 1;
						if (next == border.end() || (next->first - iter->second).sqlen() > .0f)
							next = border.begin();

						if (dot(pl.first.normal, nor(cross(next->second - iter->first, iter->second - iter->first))) >= .0f) {
							if (nor(cross(next->second - iter->first, iter->second - iter->first)).sqlen() > .0f) {
								vertex v;
								v.position = iter->first;
								next_iter.push_back(v);
								v.position = iter->second;
								next_iter.push_back(v);
								v.position = next->second;
								next_iter.push_back(v);
							}
							next->first = iter->first;
							iter = border.erase(iter);
							if (border.size() == 0)
								break;
							if (iter == border.end())
								iter = border.begin();
							changes = true;
						}
						++iter;
					}
					//if (!changes)
					//	break;
				}
				cut_mesh = next_iter;
				cout << "plane done" << endl;
			}
			building_verts.insert(building_verts.end(), cut_mesh.begin(), cut_mesh.end());
		}

		for (auto& b : building_verts)
			b.position = model_to_world(b.position);

		city_verts.insert(city_verts.end(), building_verts.begin(), building_verts.end());
	}

	//cout << "things broken" << endl;

	for (int i = 0; i < city_verts.size(); i += 3) {
		nor n = nor(cross(city_verts[i + 2].position - city_verts[i].position, city_verts[i + 1].position - city_verts[i].position)).normalized();
		for (int j = 0; j < 3; ++j)
			city_verts[i + j].normal = n;
	}

	mesh city;

	city.data(city_verts.size(), sizeof(vertex), city_verts.data());
	city.pointer(0, 0, 3); // position
	city.pointer(1, 3, 3); // normal

	trf cam_to_world;
	trf projection = trf::perspective(float(win.height) / float(win.width), .6f, .1f, 200.0f);

	camera.y = 4.0;

	track.seek(.0);
	track.play();


	while (win.loop() && !track.ended()) {
		glBeginQuery(GL_TIME_ELAPSED, query);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glPolygonMode(GL_FRONT, GL_LINE);

		if (win.keyHit[VK_SPACE]) {
			flymode = !flymode;
			if (flymode) {
				set_mouse();
				ShowCursor(0);
			}
			else
				ShowCursor(1);
		}

		if (flymode) {

			auto a = set_mouse();
			dirx += -.01f*a.first;
			diry += -.01f*a.second;

			cam_to_world = trf::translate(camera)*trf::rotate(dirx, vec(.0f, 1.0f, .0f))*trf::rotate(diry, vec(1.0f, .0f, .0f));
			camera += .1f*pos(cam_to_world(vec(win.keyDown[0x44] - win.keyDown[0x41], 0, win.keyDown[0x57] - win.keyDown[0x53])));
		}

		while (t < track.getTime()) {
			t += 1.0f / 60.0f;
		}

		trf model_to_world;

		trf world_to_cam = cam_to_world.inverse();
		trf cam_to_clip = projection;

		default.use();
		default.setUniform("model_to_world", model_to_world.m.m, GMATRIX4);
		default.setUniform("cam_to_clip", cam_to_clip.m.m, GMATRIX4);
		default.setUniform("world_to_cam", world_to_cam.m.m, GMATRIX4);

		city.draw(GL_TRIANGLES);

		glEndQuery(GL_TIME_ELAPSED);
		glGetQueryObjectiv(query, GL_QUERY_RESULT, &res);
		GLuint err = glGetError();
		if (err) system("pause");
		if (err || (loops % 60) == 0) {
			printf("frametime: %.2lfms\n", double(res) / 1000000.0);
			printf("gl error: 0x%X\n", err);
			printf("time: %f\n", t);
			if (err) system("pause");
		}
		++loops;
	}

	//ExitProcess(0);
	return 0;
}
