package com.example.subway;

import android.content.Context;
import android.content.Intent;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Typeface;
import android.os.Bundle;
import android.view.View;
import android.widget.ScrollView;

import androidx.appcompat.app.AppCompatActivity;

public class Subway extends AppCompatActivity {
    public static String stn_name[] = {
            "신창","온양온천","배방","아산","쌍용","봉명","천안","두정","직산","성환",
            "평택","지제","서정리","송탄","진위","오산","오산대","세마","서동탄","병점","세류",
            "수원","화서","성균관대","의왕","당정","군포","금정","명학","안양","관악","석수",
            "광명","금천구청","독산","가산디지털단지","구로","신도림","영등포","신길",
            "대방","노량진","용산","남영","서울역","시청","종각","종로3가","종로5가","동대문",
            "동묘앞","신설동","제기동","청량리","회기","외대앞","신이문","석계","광운대",
            "월계","녹천","창동","방학","도봉","도봉산","망월사","회룡","의정부","가능","녹양",
            "양주","덕계","덕정","지행","동두천중앙","보산","동두천","소요산",
            "구일","개봉","오류동","온수","역곡","소사","부천","중동","송내","부개",
            "부평","백운","동암","간석","주안","도화","제물포","도원","동인천","인천",   //1호선 = 98개

            "까치산","신정네거리","양천구청","도림천","문래","영등포구청",
            "당산","합정","홍대입구","신촌","이대","아현","충정로","을지로입구",
            "을지로3가","을지로4가","동대문역사문화공원","신당","상왕십리","왕십리",
            "한양대","뚝섬","성수","건대입구","구의","강변","잠실나루","잠실","잠실새내",
            "종합운동장","삼성","선릉","역삼","강남","교대","서초","방배","사당","낙성대",
            "서울대입구","봉천","신림","신대방","구로디지털단지","대림",//=46
            "용답","신답","용두",   //2호선  = 51개

            "오금","경찰병원","가락시작","수서","일원","대청","학여울","대치","도곡",
            "매봉","양재","남부터미널","고속터미널","잠원","신사","압구정","옥수",
            "금호","약수","동대입구","충무로","안국","경복궁",
            "독립문","무악재","홍제","녹번","불광","연신내","구파발","지축","삼송","원흥",
            "원당","화정","대곡","백석","마두","정발산","주엽","대화",   //3호선 44개

            "오이도","정왕","신길온천","안산","초지","고잔","중앙","한대앞","상록수",
            "반월","대야미","수리산","산본","범계","평촌","인덕원","정부과천청사",
            "과천","대공원","경마공원","선바위","남태령","총신대입구","동작","이촌",
            "신용산","삼각지","숙대입구","회현","명동",
            " 혜화","한성대입구","성신여대입구","길음","미아사거리","미아","수유","쌍문","노원","상계","당고개",       //4호선 = 48개

            "방화","개화산","김포공항","송정","마곡","발산","우장산","화곡",
            "신정","목동","오목교","양평", "영등포시장", " 여의도",
            "여의나루","마포","공덕","애오개","서대문","광화문",
            " 청구","신금호","행당","마장",
            "답십리","장한평","군자","아차산","광나루","천호","강동","둔촌동","올림픽공원","방이","개롱","거여","마천",//길동->강동+8
            "길동","굽은다리","명일","고덕","상일동",       //5호선 = 51개

            "봉화산","화랑대","태릉입구","돌곶이","상월곡","월곡","고려대",
            "안암","보문","창신","버티고개","한강진","이태원",
            "녹사평","효창공원앞","대흥","광흥창","상수","망원",
            "마포구청","월드컵경기장","디지털미디어시티","증산","새절","응암","역촌",
            "독바위","구산",   //응암->구산-5   //6호선 = 38개

            "부평구청","굴포천","삼산체육관","상동","부천시청","신중동","춘의",
            "부천종합운동장","까치울","천왕","광명사거리","철산",
            "남구로","신풍","보라매","신대방삼거리","장승배기","상도","숭실대입구",
            "남성","내방","반포","논현","학동","강남구청","청담",
            "뚝섬유원지","어린이대공원","군자","용마산","사가정","면목",
            "상봉","중화","먹골","공릉","하계","중계","마들","수락산",
            "장암",      //7호선 = 51개

            "모란","수진","신흥","단대오거리","남한산성입구","산성","장지","복정",
            "문정","송파","석촌","몽촌토성","강동구청","암사",   //8호선 17개

            "개화","공항시장","신방화","마곡나루","양천향교","가양","증미",
            "등촌","염창","신목동","선유도","국회의사당","샛강",
            "노들","흑석","구반포","신반포","사평","신논현","언주",
            "선정릉","삼성중앙","봉은사","삼전","석촌고분","송파나루",
            "한성백제","둔촌오륜", "중앙보훈병원"   //9호선 = 38개
    };

    static {
        System.loadLibrary("native-lib");
    }

    String start, end;
    ScrollView sv;
    public static int result[];
    String s[];
    int height;
    public static int lineArray[];
    public static int total[];

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //setContentView(new MyView(this));
        setContentView(R.layout.activity_subway);
        Intent intent = getIntent();
        start = intent.getExtras().getString("출발지");
        end = intent.getExtras().getString("도착지");
        height = intent.getExtras().getInt("세로");

        lineArray = new int[382];
        total = new int[382];

        try {
            result = shortest_path(start, end, lineArray, total);
        }
        catch(Exception e) {
            System.out.println("오류다");
        }

        for(int i=result.length-1; i>=0; i--) {
            System.out.println(stn_name[result[i]]);
        }

        sv = (ScrollView)findViewById(R.id.sv);

        MyView myView = new MyView(this);
        sv.addView(myView);
    }
    public int getSize() {
        try {
            return result.length;
        } catch(Exception e) {
            return 0; }
    }
    public int[] getResult() {
        return result;
    }
    public String[] getName() {
        return stn_name;
    }
    public int[] getLine() {
        return lineArray;
    }
    public int[] getTotal() {
        return total;
    }

    public native int[] shortest_path(String start, String end, int[] lineArray, int total[]);
}

class MyView extends View {
    Subway s = new Subway();

    public MyView(Context context) {
        super(context);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        Paint paint = new Paint(); // 화면에 그려줄 도구를 셋팅하는 객체

        setBackgroundColor(Color.WHITE); // 배경색을 지정

        paint.setStrokeWidth(10f);    // 선의 굵기
        paint.setStyle(Paint.Style.FILL);

        paint.setColor(Color.BLACK); // 색상을 지정
        paint.setTextSize(70);
        paint.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.BOLD));
        canvas.drawText("총 소요시간은 " + s.getTotal()[s.getResult()[0]]+"분 입니다." , 170, 150, paint);
        paint.setColor(Color.BLUE); // 색상을 지정
        paint.setTextSize(50);
        paint.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.NORMAL));
        int x = s.getSize();
        for(int i=0; i<s.getSize(); i++) {
            canvas.drawCircle(300, 300+i*200, 40, paint);
            canvas.drawText(s.getName()[s.getResult()[--x]] + " (" + s.getLine()[i] + "호선)" , 400, 330+i*200, paint);
            if(i!=0) {
                canvas.drawLine(300, 100+i*200, 300, 300+i*200, paint); // 직선
            }
        }
    }

    protected void onMeasure (int widthMeasureSpec, int heightMeasureSpec){
        setMeasuredDimension(1080, 250*s.getSize());  // 뷰의 크기를 폭 2560, 높이 2560으로 강제로 지정
    }
}