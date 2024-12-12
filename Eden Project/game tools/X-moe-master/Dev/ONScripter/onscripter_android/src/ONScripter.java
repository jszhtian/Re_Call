package cn.natdon.onscripter;

import java.io.File;
import java.io.FileFilter;
import java.io.InputStream;
import java.io.FileOutputStream;
import java.io.BufferedOutputStream;
import java.util.Arrays;
import java.util.Comparator;
import java.lang.SecurityException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Locale;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.ComponentName;
import android.content.DialogInterface;
import android.content.res.AssetManager;
import android.content.res.AssetFileDescriptor;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.content.Intent;
import android.content.Intent.ShortcutIconResource;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.content.Context;
import android.graphics.Color;
import android.graphics.Rect;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnLongClickListener;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.Window;
import android.view.WindowManager;
import android.view.Display;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.SubMenu;
import android.view.MenuItem;
import android.view.Gravity;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.os.PowerManager;
import android.os.Environment;
import android.os.Message;
import android.os.Handler;
import android.widget.TextView;
import android.widget.ListView;
import android.widget.ArrayAdapter;
import android.widget.SimpleAdapter;
import android.widget.AdapterView;
import android.widget.CheckBox;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.HorizontalScrollView;
import android.util.Log;
import android.util.DisplayMetrics;
import android.net.Uri;
import android.graphics.Bitmap.Config;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.widget.ImageView;
import android.widget.PopupWindow;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;

public class ONScripter extends Activity implements
		AdapterView.OnItemClickListener, Runnable {
	// Launcher contributed by katane-san

	private File mCurrentDirectory = null;
	private File mOldCurrentDirectory = null;
	private File[] mDirectoryFiles = null;
	private ListView listView = null;
	private int num_file = 0;
	private byte[] buf = null;
	private int screen_w, screen_h;
	private int button_w, button_h;
	private Button btn1, btn2, btn3, btn4, btn5, btn6, btn9, btn10, btn11,
			btnOK;
	private LinearLayout layout = null;
	private LinearLayout layout1 = null;
	private LinearLayout layout2 = null;
	private LinearLayout layout3 = null;
	private ScrollView SV = null;
	private HorizontalScrollView HSV = null;
	private HorizontalScrollView TSV = null;
	private boolean mIsLandscape = true;
	private boolean mButtonVisible = true;
	private boolean mScreenCentered = false;
	private boolean set_checkWS = false;
	private boolean set_checkSP = false;
	private boolean set_OtherPL = false;
	private boolean set_checkDR = false;
	private boolean set_keepON = false;
	private SimpleAdapter listItemAdapter;
	private ArrayList<HashMap<String, Object>> listItems;
	private HashMap<String, Object> map;
	public static WindowManager.LayoutParams params = new WindowManager.LayoutParams();
	private WindowManager wm;
	private LinearLayout Btnlayout;
	private static final String SHORT_CUT_EXTRAS = "cn.natdon.onscripter";

	static class FileSort implements Comparator<File> {
		public int compare(File src, File target) {
			return src.getName().compareTo(target.getName());
		}
	}

	private void setupDirectorySelector() {
		mDirectoryFiles = mCurrentDirectory.listFiles(new FileFilter() {
			public boolean accept(File file) {
				return (!file.isHidden() && file.isDirectory());
			}
		});

		Arrays.sort(mDirectoryFiles, new FileSort());

		int length = mDirectoryFiles.length;
		if (mCurrentDirectory.getParent() != null)
			length++;
		String names;

		int j = 0;
		Drawable d = null;

		listItems = new ArrayList<HashMap<String, Object>>();

		if (mCurrentDirectory.getParent() != null) {
			names = "..";
			map = new HashMap<String, Object>();
			map.put("ItemTitle", "..");
			listItems.add(map);
		}
		for (int i = 0; i < mDirectoryFiles.length; i++) {
			names = mDirectoryFiles[i].getName();

			String filePath = mDirectoryFiles[i].toString() + "/ICON.PNG";
			File logo = new File(filePath);
			if (mDirectoryFiles[i].isDirectory() && logo.exists()) {

			}

			map = new HashMap<String, Object>();

			map.put("ItemTitle", "   " + names); // 文字
			map.put("ItemImage", filePath); // 图片
			listItems.add(map);
		}
		listItemAdapter = new SimpleAdapter(this, listItems, // listItems数据�?				R.layout.list_item, // ListItem的XML布局实现
				new String[] { "ItemTitle", "ItemImage" }, // 动态数组与ImageItem对应的子�?				new int[] { R.id.ItemTitle, R.id.ItemImage });
		// ArrayAdapter<String> arrayAdapter = new ArrayAdapter<String>(this,
		// android.R.layout.simple_list_item_1, names);

		listView.setAdapter(listItemAdapter);
		listView.setOnItemClickListener(this);
	}

	private void runLauncher() {
		mCurrentDirectory = new File(gCurrentDirectoryPath);
		if (mCurrentDirectory.exists() == false) {
			if (!android.os.Environment.getExternalStorageState().equals(
					android.os.Environment.MEDIA_MOUNTED)) {
				gCurrentDirectoryPath = Environment.getRootDirectory()
						.getPath();
				mCurrentDirectory = new File(gCurrentDirectoryPath);
			} else {
				gCurrentDirectoryPath = Environment
						.getExternalStorageDirectory().getPath();
				mCurrentDirectory = new File(gCurrentDirectoryPath);
			}

			if (mCurrentDirectory.exists() == false)
				showErrorDialog("Could not find SD card.");
		}

		listView = new ListView(this);
		listView.setBackgroundColor(0xf5f5f5f5);
		listView.setCacheColorHint(0x00000000);
		listView.setPersistentDrawingCache(ListView.PERSISTENT_ALL_CACHES);
		listView.setScrollingCacheEnabled(true);
		listView.setAlwaysDrawnWithCacheEnabled(true);
		Drawable d = Drawable.createFromPath("/sdcard/ons/bg.png");
		if (d != null) {
			listView.setBackgroundDrawable(d);
		}

		LinearLayout layoutH = new LinearLayout(this);
		// layoutH.setBackgroundColor(Color.rgb(244,244,255));
		LinearLayout layoutTH = new LinearLayout(this);
		TSV = new HorizontalScrollView(this);

		final LinearLayout Slayout = new LinearLayout(this);
		LinearLayout SLlayout = new LinearLayout(this);
		ScrollView BtnSV = new ScrollView(this);

		about = new TextView(this);
		about.setText("ONScripter-CN   关于   ");
		about.setTextSize(21);
		about.setGravity(Gravity.CENTER_VERTICAL);
		about.setBackgroundColor(Color.argb(0, 0, 0, 0));
		about.setTextColor(Color.BLACK);
		about.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				About();
			}
		});
		layoutH.addView(about);

		ONSSetting = new TextView(this);
		ONSSetting.setText("   设置   ");
		ONSSetting.setTextSize(21);
		ONSSetting.setGravity(Gravity.CENTER_VERTICAL);
		ONSSetting.setBackgroundColor(Color.argb(0, 0, 0, 0));
		ONSSetting.setTextColor(Color.BLACK);
		ONSSetting.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				if (m_popupWindow != null && m_popupWindow.isShowing()) {
					m_popupWindow.dismiss();
				} else {
					
					m_popupWindow = new PopupWindow(Slayout, LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT,true);
					m_popupWindow.setTouchable(true);
					m_popupWindow.setOutsideTouchable(true);
					m_popupWindow.setBackgroundDrawable(new BitmapDrawable());
					m_popupWindow.showAsDropDown(ONSSetting,5,5);
				}
			}
		});
		layoutH.addView(ONSSetting);

		Repair = new TextView(this);
		Repair.setText("   修复快捷方式透明   ");
		Repair.setTextSize(21);
		Repair.setGravity(Gravity.CENTER_VERTICAL);
		Repair.setBackgroundColor(Color.argb(0, 0, 0, 0));
		Repair.setTextColor(Color.BLACK);
		Repair.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				addShortcut("删除", "");
			}
		});
		SLlayout.addView(Repair);

		Language = new TextView(this);
		Language.setText(lg);
		Language.setTextSize(21);
		Language.setGravity(Gravity.CENTER_VERTICAL);
		Language.setBackgroundColor(Color.argb(0, 0, 0, 0));
		Language.setTextColor(Color.BLACK);
		Language.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				if (mylanguage) {
					Language.setText("   "+"中文");
					mylanguage = false;
					
				} else {
					Language.setText("   "+"English");
					mylanguage = true;
					
				}

				Editor e = getSharedPreferences("setlanguage", MODE_PRIVATE)
						.edit();
				e.putString("language", Language.getText().toString());
				e.putBoolean("sellanguage", mylanguage);
				e.commit();
			}
		});
		SLlayout.addView(Language);

		SetOrientation = new TextView(this);
		SetOrientation.setText(myOrientation);
		SetOrientation.setTextSize(21);
		SetOrientation.setGravity(Gravity.CENTER_VERTICAL);
		SetOrientation.setBackgroundColor(Color.argb(0, 0, 0, 0));
		SetOrientation.setTextColor(Color.BLACK);
		SetOrientation.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				if (!Orientation) {
					SetOrientation.setText("   "+"重力旋转屏幕");
					Orientation = true;
					
				} else {
					SetOrientation.setText("   "+"锁定屏幕");
					Orientation = false;
					
				}

				Editor e = getSharedPreferences("setlanguage", MODE_PRIVATE)
						.edit();
				e.putString("myOrientation", SetOrientation.getText().toString());
				e.putBoolean("Orientation", Orientation);
				e.commit();
			}
		});
		SLlayout.addView(SetOrientation);

		BtnSV.addView(SLlayout);
		Slayout.addView(BtnSV);
		SLlayout.setBackgroundResource(R.drawable.popupwindow);
		Slayout.setBackgroundResource(R.drawable.popupwindow);
		SLlayout.setOrientation(LinearLayout.VERTICAL);
		Slayout.setOrientation(LinearLayout.VERTICAL);

		TSV.addView(layoutH);
		layoutTH.addView(TSV, new LinearLayout.LayoutParams(
				LinearLayout.LayoutParams.FILL_PARENT,
				LinearLayout.LayoutParams.FILL_PARENT, 1.0f));

		listView.addHeaderView(layoutTH, null, false);

		setupDirectorySelector();

		setContentView(listView);
	}

	public void onItemClick(AdapterView<?> parent, View v, int position, long id) {
		position--; // for header

		TextView textView = (TextView) v.findViewById(R.id.ItemTitle);
		mOldCurrentDirectory = mCurrentDirectory;

		if (textView.getText().equals("..")) {
			mCurrentDirectory = new File(mCurrentDirectory.getParent());
			gCurrentDirectoryPath = mCurrentDirectory.getPath();
		} else {
			if (mCurrentDirectory.getParent() != null)
				position--;
			gCurrentDirectoryPath = mDirectoryFiles[position].getPath();
			mCurrentDirectory = new File(gCurrentDirectoryPath);
		}

		mDirectoryFiles = mCurrentDirectory.listFiles(new FileFilter() {
			public boolean accept(File file) {
				return (file.isFile() && (file.getName().equals("0.txt")
						|| file.getName().equals("00.txt")
						|| file.getName().equals("nscr_sec.dat")
						|| file.getName().equals("nscript.___") || file
						.getName().equals("nscript.dat")));
			}
		});

		if (mDirectoryFiles.length == 0) {
			setupDirectorySelector();
		} else {
			mDirectoryFiles = mCurrentDirectory.listFiles(new FileFilter() {
				public boolean accept(File file) {
					return (file.isFile()
							&& (file.getName().equals("default.ttf"))
							|| (file.getName().equals("DEFAULT.TTF"))
							|| (file.getName().equals("DEFAULT.ttf")) || (file
							.getName().equals("default.TTF")));
				}
			});

			if (mDirectoryFiles.length == 0) {
				alertDialogBuilder.setTitle(getString(R.string.app_name));
				alertDialogBuilder.setMessage("default.ttf is missing.");
				alertDialogBuilder.setPositiveButton("OK",
						new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog,
									int whichButton) {
								setResult(RESULT_OK);
							}
						});
				AlertDialog alertDialog = alertDialogBuilder.create();
				alertDialog.show();

				mCurrentDirectory = mOldCurrentDirectory;
				setupDirectorySelector();
			} else {

				Dialog(textView.getText().toString().trim(),gCurrentDirectoryPath);
			}
		}
	}

	public void Dialog(final String myname, final String path) {
		LayoutInflater inflater = getLayoutInflater();
		View layout = inflater.inflate(R.layout.dialog,
				(ViewGroup) findViewById(R.id.dialog));
		LinearLayout linearlayout = (LinearLayout) layout
				.findViewById(R.id.mylinearlayout);
		LinearLayout layoutDL = new LinearLayout(this);
		HorizontalScrollView DLTSV = new HorizontalScrollView(this);
		final AlertDialog.Builder builder2 = new AlertDialog.Builder(this);

		builder2.setCancelable(false);
		builder2.setIcon(R.drawable.icon);
		builder2.setTitle("ONScripter");
		builder2.setMessage("设置" + "�? );

		/*btnOK = new Button(this);
		btnOK.setTextColor(Color.BLACK);
		btnOK.setText("确定");
		btnOK.setTextSize(18);
		btnOK.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				WriteSetting(myname);
				ReadSetting(myname);
			}
		});
		layoutDL.addView(btnOK);*/

		checkWS = new CheckBox(this);
		checkWS.setText("宽屏");
		checkWS.setBackgroundColor(Color.rgb(244, 244, 255));
		checkWS.setTextColor(Color.BLACK);
		layoutDL.addView(checkWS);

		checkSP = new CheckBox(this);
		checkSP.setText("屏蔽视频");
		checkSP.setBackgroundColor(Color.rgb(244, 244, 255));
		checkSP.setTextColor(Color.BLACK);
		layoutDL.addView(checkSP);

		OtherPL = new CheckBox(this);
		OtherPL.setText("外部播放�?);
		OtherPL.setBackgroundColor(Color.rgb(244, 244, 255));
		OtherPL.setTextColor(Color.BLACK);
		layoutDL.addView(OtherPL);

		checkDR = new CheckBox(this);
		checkDR.setText("关闭缩放");
		checkDR.setBackgroundColor(Color.rgb(244, 244, 255));
		checkDR.setTextColor(Color.BLACK);
		layoutDL.addView(checkDR);

		keepON = new CheckBox(this);
		keepON.setText("长亮 ");
		keepON.setBackgroundColor(Color.rgb(244, 244, 255));
		keepON.setTextColor(Color.BLACK);
		layoutDL.addView(keepON);
		DLTSV.addView(layoutDL);
		linearlayout.addView(DLTSV);
		builder2.setView(layout);

		SharedPreferences checkset = getSharedPreferences(myname, MODE_PRIVATE);
		boolean ck = checkset.contains("checkWS");
		if (!ck) {
			Editor e = getSharedPreferences(myname, MODE_PRIVATE).edit();

			set_checkWS = false;
			e.putBoolean("checkWS", set_checkWS);

			set_checkSP = false;
			e.putBoolean("checkSP", set_checkSP);

			set_OtherPL = false;
			e.putBoolean("OtherPL", set_OtherPL);

			set_checkDR = false;
			e.putBoolean("checkDR", set_checkDR);

			set_keepON = false;
			e.putBoolean("keepON", set_keepON);

			e.commit();
		}
		ReadSetting(myname);

		builder2.setPositiveButton("运行游戏",
				new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int whichButton) {
						WriteSetting(myname);
						ReadSetting(myname);
						runSDLApp();
						dialog.dismiss();

					}
				});
		builder2.setNegativeButton("创建快捷方式",
				new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int whichButton) {
						WriteSetting(myname);
						ReadSetting(myname);
						addShortcut(myname, path);
						
						rest();
						dialog.dismiss();
					}
				});
		builder2.setNeutralButton("取消", new DialogInterface.OnClickListener() {
			public void onClick(DialogInterface dialog, int whichButton) {
				rest();
				dialog.dismiss();
			}
		});

		builder2.create().show();

	}

	public void rest() {
		mOldCurrentDirectory = mCurrentDirectory;
		mCurrentDirectory = new File(mCurrentDirectory.getParent());
		gCurrentDirectoryPath = mCurrentDirectory.getPath();
		setupDirectorySelector();
	}

	public void addShortcut(String name, String path) {
		Intent shortcut = new Intent(
				"com.android.launcher.action.INSTALL_SHORTCUT");
		shortcut.putExtra(Intent.EXTRA_SHORTCUT_NAME, name);
		shortcut.putExtra("duplicate", false);
		ComponentName comp = new ComponentName(this.getPackageName(), "."
				+ this.getLocalClassName());
		shortcut.putExtra(Intent.EXTRA_SHORTCUT_INTENT, new Intent(
				Intent.ACTION_MAIN).setComponent(comp));
		Intent shortcutIntent = new Intent(Intent.ACTION_MAIN);
		shortcutIntent.setClass(this, start.class);
		shortcutIntent.putExtra(SHORT_CUT_EXTRAS, path);
		shortcutIntent.putExtra("setting", name);
		shortcut.putExtra(Intent.EXTRA_SHORTCUT_INTENT, shortcutIntent);

		String iconPath = path + "/ICON.PNG";
		Drawable d = Drawable.createFromPath(iconPath);
		if (d != null) {
			shortcut.putExtra(Intent.EXTRA_SHORTCUT_ICON,
					generatorContactCountIcon(((BitmapDrawable) (getResources()
							.getDrawable(R.drawable.icon))).getBitmap(), d));
		} else {
			ShortcutIconResource iconRes = Intent.ShortcutIconResource
					.fromContext(this, R.drawable.icon);
			shortcut.putExtra(Intent.EXTRA_SHORTCUT_ICON_RESOURCE, iconRes);
		}
		sendBroadcast(shortcut);
	}

	public Bitmap generatorContactCountIcon(Bitmap icon, Drawable d) {
		// 初始化画�?		int iconSize = (int) getResources().getDimension(
				android.R.dimen.app_icon_size);
		Bitmap contactIcon = Bitmap.createBitmap(iconSize, iconSize,
				Config.ARGB_8888);
		Canvas canvas = new Canvas(contactIcon);
		BitmapDrawable bd = (BitmapDrawable) d;
		Bitmap bm = bd.getBitmap();

		// 拷贝图片
		Paint iconPaint = new Paint();
		iconPaint.setDither(true);
		iconPaint.setFilterBitmap(true);
		Rect src = new Rect(0, 0, icon.getWidth(), icon.getHeight());
		Rect dst = new Rect(0, 0, iconSize, iconSize);
		canvas.drawBitmap(icon, src, dst, iconPaint);

		Paint countPaint = new Paint(Paint.ANTI_ALIAS_FLAG
				| Paint.FILTER_BITMAP_FLAG);

		Display disp = ((WindowManager) this
				.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
		int dh = disp.getHeight();
		if (dh <= 320)
			canvas.drawBitmap(CreatMatrixBitmap(bm, 48, 48), src, dst,
					countPaint);
		if (dh < 600)
			canvas.drawBitmap(CreatMatrixBitmap(bm, 72, 72), src, dst,
					countPaint);
		if (dh > 600)
			canvas.drawBitmap(CreatMatrixBitmap(bm, 96, 96), src, dst,
					countPaint);

		return contactIcon;
	}

	private Bitmap CreatMatrixBitmap(Bitmap bitMap, float scr_width,
			float res_height) {

		int bitWidth = bitMap.getWidth();
		int bitHeight = bitMap.getHeight();
		float scaleWidth = scr_width / (float) bitWidth;
		float scaleHeight = res_height / (float) bitHeight;
		Matrix matrix = new Matrix();
		matrix.postScale(scaleWidth, scaleHeight);
		bitMap = Bitmap.createBitmap(bitMap, 0, 0, bitWidth, bitHeight, matrix,
				true);
		return bitMap;
	}

	public void Setlanguage() {

		Resources resources = getResources();
		Configuration config = resources.getConfiguration();
		DisplayMetrics dm = resources.getDisplayMetrics();
		if (mylanguage){
			config.locale = Locale.ENGLISH;
		}
		else
			config.locale = Locale.SIMPLIFIED_CHINESE;
		resources.updateConfiguration(config, dm);
	}

	public void WriteSetting(String Gamename) {
		Editor e = getSharedPreferences(Gamename, MODE_PRIVATE).edit();
		if (checkWS.isChecked()) {
			set_checkWS = true;
			e.putBoolean("checkWS", set_checkWS);
		} else {
			set_checkWS = false;
			e.putBoolean("checkWS", set_checkWS);
		}
		if (checkSP.isChecked()) {
			set_checkSP = true;
			e.putBoolean("checkSP", set_checkSP);
		} else {
			set_checkSP = false;
			e.putBoolean("checkSP", set_checkSP);
		}
		if (OtherPL.isChecked()) {
			set_OtherPL = true;
			e.putBoolean("OtherPL", set_OtherPL);
		} else {
			set_OtherPL = false;
			e.putBoolean("OtherPL", set_OtherPL);
		}
		if (checkDR.isChecked()) {
			set_checkDR = true;
			e.putBoolean("checkDR", set_checkDR);
		} else {
			set_checkDR = false;
			e.putBoolean("checkDR", set_checkDR);
		}
		if (keepON.isChecked()) {
			set_keepON = true;
			e.putBoolean("keepON", set_keepON);
		} else {
			set_keepON = false;
			e.putBoolean("keepON", set_keepON);
		}
		e.commit();
	}

	public void ReadSetting(String setting) {
		SharedPreferences spset = getSharedPreferences(setting, MODE_PRIVATE);// 读取设置
		set_checkWS = spset.getBoolean("checkWS", set_checkWS);
		if (set_checkWS) {
			checkWS.setChecked(true);
		}
		set_checkSP = spset.getBoolean("checkSP", set_checkSP);
		if (set_checkSP) {
			checkSP.setChecked(true);
		}
		set_OtherPL = spset.getBoolean("OtherPL", set_OtherPL);
		if (set_OtherPL) {
			OtherPL.setChecked(true);
		}
		set_checkDR = spset.getBoolean("checkDR", set_checkDR);
		if (set_checkDR) {
			checkDR.setChecked(true);
		}
		set_keepON = spset.getBoolean("keepON", set_keepON);
		if (set_keepON) {
			keepON.setChecked(true);
		}
		gDisableVideo = checkSP.isChecked();
		gDisableRescale = checkDR.isChecked();
		gOtherPL = OtherPL.isChecked();
		gWideScreen = checkWS.isChecked();
		if (keepON.isChecked()) {
			getWindow()
					.addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		}
	}

	public void FSetting(String setting) {
		SharedPreferences spset = getSharedPreferences(setting, MODE_PRIVATE);// 读取设置
		set_checkWS = spset.getBoolean("checkWS", set_checkWS);
		if (set_checkWS)
			gWideScreen = true;
		set_checkSP = spset.getBoolean("checkSP", set_checkSP);
		if (set_checkSP)
			gDisableVideo = true;
		set_OtherPL = spset.getBoolean("OtherPL", set_OtherPL);
		if (set_OtherPL)
			gOtherPL = true;
		set_checkDR = spset.getBoolean("checkDR", set_checkDR);
		if (set_checkDR)
			gDisableRescale = true;
		set_keepON = spset.getBoolean("keepON", set_keepON);
		if (set_keepON) {
			gKeepON = true;
			if (gKeepON)
				getWindow().addFlags(
						WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		}
	}

	public void About() {
		AlertDialog.Builder builder = new AlertDialog.Builder(
				cn.natdon.onscripter.ONScripter.this);
		builder.setTitle("关于 updates by natdon");
		builder.setMessage(getResources().getString(R.string.info));
		builder.setPositiveButton("确定", null);
		builder.setNegativeButton("访问论坛",
				new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int whichButton) {
						Uri uri = Uri.parse("http://bbs.acgfuture.com");
						Intent web = new Intent(Intent.ACTION_VIEW, uri);
						startActivity(web);
					}
				});

		builder.create().show();
	}

	private void runDownloader() {
		File file = new File(gCurrentDirectoryPath + "/"
				+ getResources().getString(R.string.download_version));
		if (file.exists() == false) {
			progDialog = new ProgressDialog(this);
			progDialog.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);
			progDialog.setMessage("Downloading archives from Internet:");
			progDialog.show();

			PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
			wakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK,
					"ONScripter");
			wakeLock.acquire();

			downloader = new DataDownloader(gCurrentDirectoryPath,
					getResources().getString(R.string.password), getResources()
							.getString(R.string.download_version),
					getResources().getString(R.string.download_url), handler);
		} else {
			runSDLApp();
		}
	}

	private void runCopier() {
		File file = new File(gCurrentDirectoryPath + "/"
				+ getResources().getString(R.string.download_version));
		if (file.exists() == false) {
			progDialog = new ProgressDialog(this);
			progDialog.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);
			progDialog.setMessage("Copying archives: ");
			progDialog.show();

			PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
			wakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK,
					"ONScripter");
			wakeLock.acquire();

			new Thread(this).start();
		} else {
			runSDLApp();
		}
	}

	@Override
	public void run() {
		num_file = 0;
		buf = new byte[8192 * 2];

		copyRecursive("");

		File file = new File(gCurrentDirectoryPath + "/"
				+ getResources().getString(R.string.download_version));
		try {
			file.createNewFile();
		} catch (Exception e) {
			sendMessage(-2, 0, "Failed to create version file: " + e.toString());
		}
		;

		sendMessage(-1, 0, null);
	}

	private void copyRecursive(String path) {
		AssetManager as = getResources().getAssets();
		try {
			File file = new File(gCurrentDirectoryPath + "/" + path);
			if (!file.exists()) {
				file.mkdir();
			}

			String[] file_list = as.list(path);
			for (String str : file_list) {
				InputStream is = null;
				String path2 = path;
				if (!path.equals(""))
					path2 += "/";
				path2 += str;

				int total_size = 0;
				try {
					is = as.open(path2);
					AssetFileDescriptor afd = as.openFd(path2);
					total_size = (int) afd.getLength();
					afd.close();
				} catch (Exception e) {
					copyRecursive(path2);
					is = null;
				}
				if (is == null)
					continue;

				File dst_file = new File(gCurrentDirectoryPath + "/" + path2);
				BufferedOutputStream os = new BufferedOutputStream(
						new FileOutputStream(dst_file));

				num_file++;
				int len = is.read(buf);
				int total_read = 0;
				while (len >= 0) {
					if (len > 0)
						os.write(buf, 0, len);
					total_read += len;
					sendMessage(total_read, total_size, "Copying archives: "
							+ num_file);

					len = is.read(buf);
					try {
						Thread.sleep(1);
					} catch (InterruptedException e) {
					}
				}
				os.flush();
				os.close();
				is.close();
			}
		} catch (Exception e) {
			progDialog.dismiss();
			sendMessage(-2, 0, "Failed to write: " + e.toString());
		}
	}

	private void runSDLApp() {
		nativeInitJavaCallbacks();

		if(Orientation)
			setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR);
		else
			setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);

		Setlanguage();

		if(mylanguage){
		String f = getResources().getString(R.string.button_rclick)+getResources().getString(R.string.menu_speed);
			byte[] bytes = f.getBytes();
			int i = bytes.length;
			int j = f.length();
			if (i != j) {
				android.os.Process.killProcess(android.os.Process.myPid());
			}
		}

		mAudioThread = new AudioThread(this);
		mGLView = new DemoGLSurfaceView(this);
		mGLView.setFocusableInTouchMode(true);
		mGLView.setFocusable(true);
		mGLView.requestFocus();
		Display disp = ((WindowManager) this
				.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
		int dw = disp.getWidth();
		int dh = disp.getHeight();

		screen_w = dw;
		screen_h = dh;
		mIsLandscape = true;
		if (gWideScreen == true) {
			if (dw * 9 >= dh * 16) {
				screen_w = dh * 16 / 9;
				button_w = dw - screen_w;
				button_h = dh / 4;
			} else {
				mIsLandscape = false;
				screen_h = dw * 9 / 16;
				button_w = dw / 4;
				button_h = dh - screen_h;
			}
		} else {
			if (dw * 3 >= dh * 4) {
				screen_w = dh * 4 / 3;
				button_w = dw - screen_w;
				button_h = dh / 4;
			} else {
				mIsLandscape = false;
				screen_h = dw * 3 / 4;
				button_w = dw / 4;
				button_h = dh - screen_h;
			}
		}

		Drawable d = Drawable.createFromPath("/sdcard/ons/btn.png");

		btn1 = new Button(this);
		btn1.setBackgroundColor(Color.rgb(10, 10, 10));
		btn1.setTextColor(Color.GRAY);
		btn1.setText(getResources().getString(R.string.button_rclick));
		if (d != null)
			btn1.setBackgroundDrawable(d);
		btn1.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				mGLView.nativeKey(KeyEvent.KEYCODE_BACK, 1);
				mGLView.nativeKey(KeyEvent.KEYCODE_BACK, 0);
			}
		});

		btn2 = new Button(this);
		btn2.setBackgroundColor(Color.rgb(10, 10, 10));
		btn2.setTextColor(Color.GRAY);
		btn2.setText(getResources().getString(R.string.button_lclick));
		if (d != null)
			btn2.setBackgroundDrawable(d);
		btn2.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				mGLView.nativeKey(KeyEvent.KEYCODE_ENTER, 1);
				mGLView.nativeKey(KeyEvent.KEYCODE_ENTER, 0);
			}
		});

		btn3 = new Button(this);
		btn3.setBackgroundColor(Color.rgb(10, 10, 10));
		btn3.setTextColor(Color.GRAY);
		btn3.setText(getResources().getString(R.string.button_up));
		if (d != null)
			btn3.setBackgroundDrawable(d);
		btn3.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				mGLView.nativeKey(KeyEvent.KEYCODE_DPAD_UP, 1);
				mGLView.nativeKey(KeyEvent.KEYCODE_DPAD_UP, 0);
			}
		});

		btn4 = new Button(this);
		btn4.setBackgroundColor(Color.rgb(10, 10, 10));
		btn4.setTextColor(Color.GRAY);
		btn4.setText(getResources().getString(R.string.button_down));
		if (d != null)
			btn4.setBackgroundDrawable(d);
		btn4.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				mGLView.nativeKey(KeyEvent.KEYCODE_DPAD_DOWN, 1);
				mGLView.nativeKey(KeyEvent.KEYCODE_DPAD_DOWN, 0);
			}
		});

		btn5 = new Button(this);
		btn5.setBackgroundColor(Color.rgb(10, 10, 10));
		btn5.setTextColor(Color.GRAY);
		btn5.setText(getResources().getString(R.string.button_left));
		if (d != null)
			btn5.setBackgroundDrawable(d);
		btn5.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				mGLView.nativeKey(KeyEvent.KEYCODE_DPAD_LEFT, 1);
				mGLView.nativeKey(KeyEvent.KEYCODE_DPAD_LEFT, 0);
			}
		});

		btn6 = new Button(this);
		btn6.setBackgroundColor(Color.rgb(10, 10, 10));
		btn6.setTextColor(Color.GRAY);
		btn6.setText(getResources().getString(R.string.button_right));
		if (d != null)
			btn6.setBackgroundDrawable(d);
		btn6.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				mGLView.nativeKey(KeyEvent.KEYCODE_DPAD_RIGHT, 1);
				mGLView.nativeKey(KeyEvent.KEYCODE_DPAD_RIGHT, 0);
			}
		});

		btn9 = new Button(this);
		btn9.setText(getResources().getString(R.string.button_vol_up));
		btn9.setBackgroundColor(Color.rgb(10, 10, 10));
		btn9.setTextColor(Color.GRAY);
		if (d != null)
			btn9.setBackgroundDrawable(d);
		btn9.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				mGLView.nativeKey(KeyEvent.KEYCODE_M, 1);
				mGLView.nativeKey(KeyEvent.KEYCODE_M, 0);
			}
		});
		btn9.setOnLongClickListener(new OnLongClickListener() {
			@Override
			public boolean onLongClick(View v) {
				mGLView.nativeKey(KeyEvent.KEYCODE_D, 1);
				mGLView.nativeKey(KeyEvent.KEYCODE_D, 0);
				return true;
			}
		});

		btn10 = new Button(this);
		btn10.setText(getResources().getString(R.string.button_vol_down));
		btn10.setBackgroundColor(Color.rgb(10, 10, 10));
		btn10.setTextColor(Color.GRAY);
		if (d != null)
			btn10.setBackgroundDrawable(d);
		btn10.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				mGLView.nativeKey(KeyEvent.KEYCODE_I, 1);
				mGLView.nativeKey(KeyEvent.KEYCODE_I, 0);
			}
		});
		btn10.setOnLongClickListener(new OnLongClickListener() {
			@Override
			public boolean onLongClick(View v) {
				mGLView.nativeKey(KeyEvent.KEYCODE_W, 1);
				mGLView.nativeKey(KeyEvent.KEYCODE_W, 0);
				return true;
			}
		});

		btn11 = new Button(this);
		btn11.setText(getResources().getString(R.string.button_menu));
		btn11.setBackgroundColor(Color.rgb(10, 10, 10));
		btn11.setTextColor(Color.GRAY);
		if (d != null)
			btn11.setBackgroundDrawable(d);
		btn11.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				openOptionsMenu();
			}
		});

		layout = new LinearLayout(this);
		layout1 = new LinearLayout(this);
		layout2 = new LinearLayout(this);
		layout3 = new LinearLayout(this);
		if (mIsLandscape)
			SV = new ScrollView(this);
		else
			HSV = new HorizontalScrollView(this);
		if (mIsLandscape)
			layout2.setOrientation(LinearLayout.VERTICAL);

		else {
			layout2.setOrientation(LinearLayout.HORIZONTAL);
			layout.setOrientation(LinearLayout.VERTICAL);
		}

		layout2.addView(btn1);
		layout2.addView(btn2);
		layout2.addView(btn3);
		layout2.addView(btn4);
		layout2.addView(btn5);
		layout2.addView(btn6);
		layout2.addView(btn9);
		layout2.addView(btn10);
		layout2.addView(btn11);
		if (mIsLandscape)
			SV.addView(layout2);
		else
			HSV.addView(layout2);
		layout.addView(layout1);
		layout.addView(mGLView, new LinearLayout.LayoutParams(screen_w,
				screen_h));
		if (mIsLandscape)
			layout.addView(SV);
		else
			layout.addView(HSV);
		layout.addView(layout3);
		resetLayout();

		setContentView(layout);

		if (wakeLock == null) {
			PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
			wakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK,
					"ONScripter");
			wakeLock.acquire();
		}
	}

	public void VirtualButton() {
		Rect frame = new Rect();
		getWindow().getDecorView().getWindowVisibleDisplayFrame(frame);
		Btnlayout = new LinearLayout(this);
		LinearLayout SVlayout = new LinearLayout(this);
		HorizontalScrollView BtnHSV = new HorizontalScrollView(this);
		wm = (WindowManager) getApplicationContext().getSystemService("window");
		Button btnup = new Button(this);
		btnup.setBackgroundColor(Color.rgb(10, 10, 10));
		btnup.setTextColor(Color.GRAY);
		btnup.setText(getResources().getString(R.string.button_up));
		btnup.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				mGLView.nativeKey(KeyEvent.KEYCODE_DPAD_UP, 1);
				mGLView.nativeKey(KeyEvent.KEYCODE_DPAD_UP, 0);
			}
		});

		Button btndown = new Button(this);
		btndown.setBackgroundColor(Color.rgb(10, 10, 10));
		btndown.setTextColor(Color.GRAY);
		btndown.setText(getResources().getString(R.string.button_down));
		btndown.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				mGLView.nativeKey(KeyEvent.KEYCODE_DPAD_DOWN, 1);
				mGLView.nativeKey(KeyEvent.KEYCODE_DPAD_DOWN, 0);
			}
		});

		Button btnleft = new Button(this);
		btnleft.setBackgroundColor(Color.rgb(10, 10, 10));
		btnleft.setTextColor(Color.GRAY);
		btnleft.setText(getResources().getString(R.string.button_lclick));
		btnleft.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				mGLView.nativeKey(KeyEvent.KEYCODE_ENTER, 1);
				mGLView.nativeKey(KeyEvent.KEYCODE_ENTER, 0);
			}
		});

		Button btnright = new Button(this);
		btnright.setBackgroundColor(Color.rgb(10, 10, 10));
		btnright.setTextColor(Color.GRAY);
		btnright.setText(getResources().getString(R.string.button_rclick));
		btnright.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				mGLView.nativeKey(KeyEvent.KEYCODE_BACK, 1);
				mGLView.nativeKey(KeyEvent.KEYCODE_BACK, 0);
			}
		});

		params.type = 2003;
		params.flags = WindowManager.LayoutParams.TYPE_APPLICATION_PANEL;

		params.width = WindowManager.LayoutParams.WRAP_CONTENT;
		params.height = WindowManager.LayoutParams.WRAP_CONTENT;
		params.alpha = 150;

		params.gravity = Gravity.LEFT | Gravity.TOP;

		params.x = 0;
		params.y = 0;
		SVlayout.addView(btnup);
		SVlayout.addView(btndown);
		SVlayout.addView(btnleft);
		SVlayout.addView(btnright);
		BtnHSV.addView(SVlayout);
		Btnlayout.addView(BtnHSV);
		wm.addView(Btnlayout, params);
	}

	public void BtnCancel() {
		if (Btnlayout != null && Btnlayout.isShown()) {
			wm = (WindowManager) getApplicationContext().getSystemService(
					"window");
			params = new WindowManager.LayoutParams();
			wm.removeView(Btnlayout);
		} else
			VirtualButton();
	}

	public void resetLayout() {
		Display disp = ((WindowManager) this
				.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
		int dw = disp.getWidth();
		int dh = disp.getHeight();

		int bw = button_w, bh = button_h;
		int w1 = 0, h1 = 0;
		int w2 = dw, h2 = dh;
		if (mIsLandscape == true) {
			if (mScreenCentered) {
				w1 = bw - bw / 2;
				bw /= 2;
			}
			if (bw > bh * 4 / 3)
				bw = bh * 4 / 3;
			h1 = dh;
			w2 = bw;
		} else {
			if (mScreenCentered) {
				h1 = bh - bh / 2;
				bh /= 2;
			}
			if (bh > bw * 3 / 4)
				bh = bw * 3 / 4;
			w1 = dw;
			h2 = bh;
		}

		btn1.setMinWidth(bw);
		btn1.setMinHeight(bh);
		btn1.setWidth(bw);
		btn1.setHeight(bh);

		btn2.setMinWidth(bw);
		btn2.setMinHeight(bh);
		btn2.setWidth(bw);
		btn2.setHeight(bh);

		btn3.setMinWidth(bw);
		btn3.setMinHeight(bh);
		btn3.setWidth(bw);
		btn3.setHeight(bh);

		btn4.setMinWidth(bw);
		btn4.setMinHeight(bh);
		btn4.setWidth(bw);
		btn4.setHeight(bh);

		btn5.setMinWidth(bw);
		btn5.setMinHeight(bh);
		btn5.setWidth(bw);
		btn5.setHeight(bh);

		btn6.setMinWidth(bw);
		btn6.setMinHeight(bh);
		btn6.setWidth(bw);
		btn6.setHeight(bh);

		btn9.setMinWidth(bw);
		btn9.setMinHeight(bh);
		btn9.setWidth(bw);
		btn9.setHeight(bh);

		btn10.setMinWidth(bw);
		btn10.setMinHeight(bh);
		btn10.setWidth(bw);
		btn10.setHeight(bh);

		btn11.setMinWidth(bw);
		btn11.setMinHeight(bh);
		btn11.setWidth(bw);
		btn11.setHeight(bh);

		if (mButtonVisible)
			layout2.setVisibility(View.VISIBLE);
		else
			layout2.setVisibility(View.INVISIBLE);

		layout.updateViewLayout(layout1, new LinearLayout.LayoutParams(w1, h1));
		if (mIsLandscape)
			layout.updateViewLayout(SV, new LinearLayout.LayoutParams(w2, h2));
		else
			layout.updateViewLayout(HSV, new LinearLayout.LayoutParams(w2, h2));
		layout.updateViewLayout(layout3, new LinearLayout.LayoutParams(dw
				- screen_w - w1 - w2, dh - screen_h - h1 - h2));
	}

	public void playVideo(char[] filename) {
		if (gDisableVideo == false) {
			try {
				String filename2 = "file:/" + gCurrentDirectoryPath + "/"
						+ new String(filename);
				filename2 = filename2.replace('\\', '/');
				Log.v("ONS", "playVideo: " + filename2);
				if (gOtherPL) {
					Uri uri = Uri.parse(filename2);
					Intent i = new Intent(Intent.ACTION_VIEW);
					i.setDataAndType(uri, "video/*");
					startActivityForResult(i, -1);
				} else {
					Intent in = new Intent();
					in.putExtra("one", filename2);
					in.setClass(ONScripter.this, VideoPlayer.class);
					ONScripter.this.startActivity(in);
				}
				overridePendingTransition(android.R.anim.fade_in,
						android.R.anim.fade_out);
			} catch (Exception e) {
				Log.e("ONS", "playVideo error:  " + e.getClass().getName());
			}
		}
	}

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		// fullscreen mode
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);
		wm = (WindowManager) getApplicationContext().getSystemService("window");

		SharedPreferences splg = getSharedPreferences("setlanguage",
				MODE_PRIVATE);
		lg = splg.getString("language", "   English");
		mylanguage = splg.getBoolean("sellanguage", true);
		myOrientation = splg.getString("myOrientation", "   锁定重力");
		Orientation = splg.getBoolean("Orientation", false);
		Setlanguage();

		gCurrentDirectoryPath = Environment.getExternalStorageDirectory()
				+ "/Android/data/" + getApplicationContext().getPackageName();
		alertDialogBuilder = new AlertDialog.Builder(this);

		SharedPreferences sp = getSharedPreferences("pref", MODE_PRIVATE);
		mButtonVisible = sp.getBoolean("button_visible", getResources()
				.getBoolean(R.bool.button_visible));
		mScreenCentered = sp.getBoolean("screen_centered", getResources()
				.getBoolean(R.bool.screen_centered));

		if (getResources().getBoolean(R.bool.use_launcher)) {
			gCurrentDirectoryPath = Environment.getExternalStorageDirectory()
					+ "/ons";
			
			final Intent intent = getIntent();
			extra = intent.getStringExtra("path");
			mysetting = intent.getStringExtra("mysetting");
			// debug.put("接收 "+extra,"onsdebug");
			if (extra != null) {
				FSetting(mysetting);
				gCurrentDirectoryPath = extra;
				runSDLApp();
			} else {

				runLauncher();
			}
			SharedPreferences sp2 = getSharedPreferences("pref", MODE_PRIVATE);
			int about = sp2.getInt("about", 1);
			if (about == 1) {
				About();
				Editor e = getSharedPreferences("pref", MODE_PRIVATE).edit();
				e.putInt("about", 2);
				e.commit();
			}
		} else if (getResources().getBoolean(R.bool.use_download))
			runDownloader();
		else
			runCopier();
	}

	@Override
	public boolean onPrepareOptionsMenu(Menu menu) {
		super.onPrepareOptionsMenu(menu);

		if (mGLView != null) {
			menu.clear();
			menu.add(Menu.NONE, Menu.FIRST, 0, getResources().getString(
					R.string.menu_automode));
			menu.add(Menu.NONE, Menu.FIRST + 1, 0, getResources().getString(
					R.string.menu_skip));
			menu.add(Menu.NONE, Menu.FIRST + 2, 0, getResources().getString(
					R.string.menu_speed));

			SubMenu sm = menu.addSubMenu(getResources().getString(
					R.string.menu_settings));
			if (mButtonVisible)
				sm.add(Menu.NONE, Menu.FIRST + 4, 0, getResources().getString(
						R.string.menu_hide_buttons));
			else
				sm.add(Menu.NONE, Menu.FIRST + 3, 0, getResources().getString(
						R.string.menu_show_buttons));

			if (mScreenCentered)
				sm.add(Menu.NONE, Menu.FIRST + 5, 0, getResources().getString(
						R.string.menu_topleft));
			else
				sm.add(Menu.NONE, Menu.FIRST + 6, 0, getResources().getString(
						R.string.menu_center));
			sm.add(Menu.NONE, Menu.FIRST + 7, 0, getResources().getString(
					R.string.menu_virtual_button));
			sm.add(Menu.NONE, Menu.FIRST + 8, 0, getResources().getString(
					R.string.menu_version));
			menu.add(Menu.NONE, Menu.FIRST + 9, 0, getResources().getString(
					R.string.menu_quit));
		}

		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		if (item.getItemId() == Menu.FIRST) {
			mGLView.nativeKey(KeyEvent.KEYCODE_A, 1);
			mGLView.nativeKey(KeyEvent.KEYCODE_A, 0);
		} else if (item.getItemId() == Menu.FIRST + 1) {
			mGLView.nativeKey(KeyEvent.KEYCODE_S, 1);
			mGLView.nativeKey(KeyEvent.KEYCODE_S, 0);
		} else if (item.getItemId() == Menu.FIRST + 2) {
			mGLView.nativeKey(KeyEvent.KEYCODE_O, 1);
			mGLView.nativeKey(KeyEvent.KEYCODE_O, 0);
		} else if (item.getItemId() == Menu.FIRST + 3) {
			mButtonVisible = true;
			resetLayout();
		} else if (item.getItemId() == Menu.FIRST + 4) {
			mButtonVisible = false;
			resetLayout();
		} else if (item.getItemId() == Menu.FIRST + 5) {
			mScreenCentered = false;
			resetLayout();
		} else if (item.getItemId() == Menu.FIRST + 6) {
			mScreenCentered = true;
			resetLayout();
		} else if (item.getItemId() == Menu.FIRST + 7) {
			BtnCancel();
		} else if (item.getItemId() == Menu.FIRST + 8) {
			alertDialogBuilder.setTitle(getResources().getString(
					R.string.menu_version));
			alertDialogBuilder.setMessage(getResources().getString(
					R.string.version));
			alertDialogBuilder.setPositiveButton("OK",
					new DialogInterface.OnClickListener() {
						public void onClick(DialogInterface dialog,
								int whichButton) {
							setResult(RESULT_OK);
						}
					});
			AlertDialog alertDialog = alertDialogBuilder.create();
			alertDialog.show();
		} else if (item.getItemId() == Menu.FIRST + 9) {

			mGLView.nativeKey(KeyEvent.KEYCODE_MENU, 2); // send SDL_QUIT
		} else {
			return false;
		}

		Editor e = getSharedPreferences("pref", MODE_PRIVATE).edit();
		e.putBoolean("button_visible", mButtonVisible);
		e.putBoolean("screen_centered", mScreenCentered);
		e.commit();

		return true;
	}

	@Override
	protected void onPause() {

		if (Btnlayout != null && Btnlayout.isShown()) {
			wm = (WindowManager) getApplicationContext().getSystemService(
					"window");
			params = new WindowManager.LayoutParams();
			wm.removeView(Btnlayout);
		}
		// TODO: if application pauses it's screen is messed up
		if (wakeLock != null)
			wakeLock.release();
		super.onPause();
		if (mGLView != null)
			mGLView.onPause();
		if (mAudioThread != null)
			mAudioThread.onPause();
	}

	@Override
	protected void onResume() {

		if (wakeLock != null && !wakeLock.isHeld())
			wakeLock.acquire();
		super.onResume();
		if (mGLView != null)
			mGLView.onResume();
		if (mAudioThread != null)
			mAudioThread.onResume();
		Setlanguage();
	}

	@Override
	protected void onStop() {
		super.onStop();
		if (mGLView != null)
			mGLView.onStop();
	}

	@Override
	protected void onDestroy() {
		if (mGLView != null)
			mGLView.exitApp();
		gCurrentDirectoryPath = null;
		extra = null;
		mysetting = null;
		super.onDestroy();
	}

	final Handler handler = new Handler() {
		public void handleMessage(Message msg) {
			int current = msg.getData().getInt("current");
			if (current == -1) {
				progDialog.dismiss();
				runSDLApp();
			} else if (current == -2) {
				progDialog.dismiss();
				showErrorDialog(msg.getData().getString("message"));
			} else {
				progDialog.setMessage(msg.getData().getString("message"));
				int total = msg.getData().getInt("total");
				if (total != progDialog.getMax())
					progDialog.setMax(total);
				progDialog.setProgress(current);
			}
		}
	};

	private void showErrorDialog(String mes) {
		alertDialogBuilder.setTitle("Error");
		alertDialogBuilder.setMessage(mes);
		alertDialogBuilder.setPositiveButton("Quit",
				new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int whichButton) {
						finish();
					}
				});
		AlertDialog alertDialog = alertDialogBuilder.create();
		alertDialog.show();
	}

	public void sendMessage(int current, int total, String str) {
		Message msg = handler.obtainMessage();
		Bundle b = new Bundle();
		b.putInt("total", total);
		b.putInt("current", current);
		b.putString("message", str);
		msg.setData(b);
		handler.sendMessage(msg);
	}

	private DemoGLSurfaceView mGLView = null;
	private AudioThread mAudioThread = null;
	private PowerManager.WakeLock wakeLock = null;
	public static String gCurrentDirectoryPath;
	public static String extra;
	public static String mysetting;
	public static String lg;
	public static String myOrientation;
	public static Boolean gDisableRescale = false;
	public static Boolean gWideScreen = false;
	public static Boolean gDisableVideo = false;
	public static Boolean gOtherPL = false;
	public static Boolean gKeepON = false;
	public static Boolean mylanguage = true;
	public static Boolean Orientation = false;
	public static CheckBox checkWS = null;
	public static CheckBox checkDR = null;
	public static CheckBox checkSP = null;
	public static CheckBox keepON = null;
	public static CheckBox OtherPL = null;
	public static TextView about, Repair, Language, ONSSetting, SetOrientation;
	public static PopupWindow m_popupWindow;

	private native int nativeInitJavaCallbacks();

	private DataDownloader downloader = null;
	private AlertDialog.Builder alertDialogBuilder = null;
	private ProgressDialog progDialog = null;

	static {
		System.loadLibrary("mad");
		System.loadLibrary("bz2");
		System.loadLibrary("tremor");
		System.loadLibrary("sdl");
		System.loadLibrary("sdl_mixer");
		System.loadLibrary("sdl_image");
		System.loadLibrary("sdl_ttf");
		System.loadLibrary("application");
		System.loadLibrary("sdl_main");
	}
}
