#include <jni.h>
#include <string>
#include <iostream>

#define TRUE 1
#define FALSE 0
#define MAX_VERTICES 382
#define MAX_TNF 53
#define INF 10000

using namespace std;

typedef struct {
    int top;
    int data[MAX_VERTICES];
} StackType;

void init_stack(StackType* s) {
    s->top = -1;
    for (int i = 0; i < MAX_VERTICES; i++) {
        s->data[i] = INF;
    }
}

int is_empty_stack(StackType* s) {
    return s->top == -1;
}

int is_full_stack(StackType* s) {
    return s->top == MAX_VERTICES - 1;
}

void push(StackType* s, int item) {
    if (!is_full_stack(s)) {
        s->data[++s->top] = item;
    }
    else {
        exit(1);
    }
}

int pop(StackType* s) {
    if (!is_empty_stack(s)) {
        return s->data[s->top--];
    }
    else {
        exit(1);
    }
}

enum Lines // 환승 중복역 제외
{
    //1호선 = 98개
    신창, 온양온천, 배방, 아산, 쌍용, 봉명, 천안, 두정, 직산, 성환,
    평택, 지제, 서정리, 송탄, 진위, 오산, 오산대, 세마, 서동탄, 병점, 세류,
    수원, 화서, 성균관대, 의왕, 당정, 군포, 금정, 명학, 안양, 관악, 석수,
    광명, 금천구청, 독산, 가산디지털단지, 구로, 신도림, 영등포, 신길,
    대방, 노량진, 용산, 남영, 서울역, 시청, 종각, 종로3가, 종로5가, 동대문,
    동묘앞, 신설동, 제기동, 청량리, 회기, 외대앞, 신이문, 석계, 광운대,
    월계, 녹천, 창동, 방학, 도봉, 도봉산, 망월사, 회룡, 의정부, 가능, 녹양,
    양주, 덕계, 덕정, 지행, 동두천중앙, 보산, 동두천, 소요산,
    // 구일=구로+30
            구일, 개봉, 오류동, 온수, 역곡, 소사, 부천, 중동, 송내, 부개,
    부평, 백운, 동암, 간석, 주안, 도화, 제물포, 도원, 동인천, 인천,
    //2호선  = 51개
            까치산, 신정네거리, 양천구청, 도림천, 문래, 영등포구청,
    당산, 합정, 홍대입구, 신촌, 이대, 아현, 충정로, 을지로입구,
    을지로3가, 을지로4가, 동대문역사문화공원, 신당, 상왕십리, 왕십리,
    한양대, 뚝섬, 성수, 건대입구, 구의, 강변, 잠실나루, 잠실, 잠실새내,
    종합운동장, 삼성, 선릉, 역삼, 강남, 교대, 서초, 방배, 사당, 낙성대,
    서울대입구, 봉천, 신림, 신대방, 구로디지털단지, 대림,//=46
    용답, 신답, 용두,
    //3호선 44개
            오금, 경찰병원, 가락시장, 수서, 일원, 대청, 학여울, 대치, 도곡,
    매봉, 양재, 남부터미널, 고속터미널, 잠원, 신사, 압구정, 옥수,
    금호, 약수, 동대입구, 충무로, 안국, 경복궁,
    독립문, 무악재, 홍제, 녹번, 불광, 연신내, 구파발, 지축, 삼송, 원흥,
    원당, 화정, 대곡, 백석, 마두, 정발산, 주엽, 대화,
    //4호선 = 48개
            오이도, 정왕, 신길온천, 안산, 초지, 고잔, 중앙, 한대앞, 상록수,
    반월, 대야미, 수리산, 산본, 범계, 평촌, 인덕원, 정부과천청사,
    과천, 대공원, 경마공원, 선바위, 남태령, 총신대입구, 동작, 이촌,
    신용산, 삼각지, 숙대입구, 회현, 명동,
    혜화, 한성대입구, 성신여대입구, 길음, 미아사거리, 미아, 수유, 쌍문, 노원, 상계, 당고개,
    //5호선 = 51개
            방화, 개화산, 김포공항, 송정, 마곡, 발산, 우장산, 화곡,
    신정, 목동, 오목교, 양평, 영등포시장, 여의도,
    여의나루, 마포, 공덕, 애오개, 서대문, 광화문,
    청구, 신금호, 행당, 마장,
    답십리, 장한평, 군자, 아차산, 광나루, 천호, 강동, 둔촌동, 올림픽공원, 방이, 개롱, 거여, 마천,//길동->강동+8
    길동, 굽은다리, 명일, 고덕, 상일동,
    //6호선 = 38개
            봉화산, 화랑대, 태릉입구, 돌곶이, 상월곡, 월곡, 고려대,
    안암, 보문, 창신, 버티고개, 한강진, 이태원,
    녹사평, 효창공원앞, 대흥, 광흥창, 상수, 망원,
    마포구청, 월드컵경기장, 디지털미디어시티, 증산, 새절, 응암, 역촌,
    독바위, 구산,   //응암->구산-5
    //7호선 = 51개
            부평구청, 굴포천, 삼산체육관, 상동, 부천시청, 신중동, 춘의,
    부천종합운동장, 까치울, 천왕, 광명사거리, 철산,
    남구로, 신풍, 보라매, 신대방삼거리, 장승배기, 상도, 숭실대입구,
    남성, 내방, 반포, 논현, 학동, 강남구청, 청담,
    뚝섬유원지, 어린이대공원, 중곡, 용마산, 사가정, 면목,
    상봉, 중화, 먹골, 공릉, 하계, 중계, 마들, 수락산,
    장암,
    //8호선 17개
            모란, 수진, 신흥, 단대오거리, 남한산성입구, 산성, 장지, 복정,
    문정, 송파, 석촌, 몽촌토성, 강동구청, 암사,
    //9호선 = 38개
            개화, 공항시장, 신방화, 마곡나루, 양천향교, 가양, 증미,
    등촌, 염창, 신목동, 선유도, 국회의사당, 샛강,
    노들, 흑석, 구반포, 신반포, 사평, 신논현, 언주, 선정릉,
    삼성중앙, 봉은사, 삼전, 석촌고분, 송파나루,
    한성백제, 둔촌오륜, 중앙보훈병원
};

string stn_name[] = {
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
int tnf_num[] =
        {
                //9호선 환승게이트
                김포공항, 당산, 여의도, 노량진, 동작, 고속터미널, 종합운동장,
                석촌, 올림픽공원,
                //8호선 환승게이트
                가락시장, 잠실, 천호,

                //7호선 환승게이트
                온수, 가산디지털단지, 대림, 총신대입구, 고속터미널, 건대입구,
                군자, 태릉입구, 노원, 도봉산,

                //6호선 환승게이트
                석계, 동묘앞, 신당, 청구, 약수, 삼각지, 공덕, 합정, 불광, 연신내, //10개

                //5호선 환승게이트
                까치산, 영등포구청, 신길, 충정로, 종로3가, 을지로4가, 동대문역사문화공원, 왕십리,   //8개

                //4호선 환승게이트
                금정, 사당, 서울역, 충무로, 동대문역사문화공원, 동대문, 창동, //7개

                //3호선 환승게이트
                종로3가, 을지로3가, 교대,   //3개

                //2호선 환승게이트
                신도림, 시청, 신설동,   //3개
        };

string tnf_name[] = {
        "김포공항"," 당산"," 여의도"," 노량진"," 동작"," 고속터미널"," 선정릉"," 종합운동장",
        "석촌"," 올림픽공원",   //10개         //9호선 환승게이트

        "모란"," 복정"," 가락시장"," 잠실"," 천호",   //5개   //8호선 환승게이트

        "온수"," 가산디지털단지"," 대림"," 총신대입구"," 고속터미널"," 강남구청"," 건대입구",

        "군자"," 태릉입구"," 노원"," 도봉산",   //11개   //7호선 환승게이트

        "석계"," 동묘앞"," 신당"," 청구"," 약수"," 삼각지"," 공덕"," 합정"," 불광"," 연신내", //10개   //6호선 환승게이트

        "까치산"," 영등포구청"," 신길"," 충정로"," 종로3가"," 을지로4가"," 동대문역사문화공원"," 왕십리",   //8개   //5호선 환승게이트

        "금정"," 사당"," 서울역"," 충무로"," 동대문역사문화공원"," 동대문"," 창동", //7개   //4호선 환승게이트

        "종로3가"," 을지로3가"," 교대",   //3개   //3호선 환승게이트

        "신도림"," 시청","신설동",   //   //2호선 환승게이트3개
//총 환승역 57개
};

/* 호선, 상행선, 하행선 */
typedef struct LineType {
    int n;			// 호선
    int updown;		// 상행 하행 구분
    LineType* link; // 리스트 초기화 필요
} LineType;

/* 일반 */
typedef struct Subway {
    string name;
    int station;				// 역
    LineType* line;				// 호선
    int duration;				// 소요시간
    //int depart[100];			// 출발시간
    //int depart_saturday[100];   // 출발시간(토요일)
    //int depart_holidayp[100];   // 출발시간(공휴일)
    Subway* link;
} Subway;

/* 급행 */
typedef struct Express {
    int station;            // 역
    int duration;            // 소요시간
    int depart[100];         // 출발시간
    int depart_saturday[100];   // 출발시간(토요일)
    int depart_holidayp[100];   // 출발시간(공휴일)
    Express* link;
} Express;

/* 그래프 */
typedef struct GraphType {
    Subway* stations[MAX_VERTICES];
    //Express* e_stations[MAX_VERTICES];
} GraphType;

static int dist[MAX_VERTICES];
static int found[MAX_VERTICES];
static int pretnf = 0;

/* 최단경로의 정점들 */
typedef struct PathType{
    int station;
    PathType* link;
} PathType;
PathType* pathlist = NULL;

/* 그래프 초기화 */
void init_graph(GraphType* g) {
    //Subway* s = NULL;
    for (int i = 0; i < MAX_VERTICES; i++) {
        g->stations[i] = NULL;
    }
}

/* 간선 삽입 */
void insert_edge(GraphType* g, int start, int end, int d, int t) {
    Subway* s, *e;
    LineType* tmp, *tmp2;
    LineType* l1,*l2;
    bool flag = false;

    // 정점 삽입
    s = (Subway*)malloc(sizeof(Subway));
    e = (Subway*)malloc(sizeof(Subway));
    s->station = end;
    s->duration = d;
    s->line = NULL;
    e->station = start;
    e->duration = d;
    e->line = NULL;

    // 호선 삽입
    tmp = (LineType*)malloc(sizeof(LineType));
    tmp2 = (LineType*)malloc(sizeof(LineType));
    tmp->n = t;
    tmp->link = NULL;
    tmp->updown = 0;
    tmp2->n = t;
    tmp2->link = NULL;
    tmp2->updown = 0;

    if (g->stations[start] == NULL) {
        s->line = tmp;
    }
    else {
        s->line = g->stations[start]->line;
        l1 = s->line;
        while (l1->link != NULL) {
            if (l1->n == t) {
                flag = true;
                break;
            }
            l1 = l1->link;
        }
        // 같은 호선이 없다면
        if (flag == false) {
            l1->link = tmp;
        }
        else {
            flag = false;
        }
    }
    s->link = g->stations[start];
    g->stations[start] = s;

    if (g->stations[end] == NULL) {
        e->line = tmp2;
    }
    else {
        e->line = g->stations[end]->line;
        l2 = e->line;
        while (l2->link != NULL) {
            if (l2->n == t) {
                flag = true;
                break;
            }
            l2 = l2->link;
        }
        if (flag == false) {
            l2->link = tmp2;
        }
        else {
            flag = false;
        }
    }
    e->link = g->stations[end];
    g->stations[end] = e;
}

/* 단방향 간선 삽입 */
void insert_edge_oneway(GraphType* g, int start, int end, int d, int t) {
    Subway* s;
    LineType* tmp;
    LineType* l1;
    bool flag = false;

    // 정점 삽입
    s = (Subway*)malloc(sizeof(Subway));
    s->station = end;
    s->duration = d;
    s->line = NULL;

    // 호선 삽입
    tmp = (LineType*)malloc(sizeof(LineType));
    tmp->n = t;
    tmp->link = NULL;
    tmp->updown = 0;


    if (g->stations[start] == NULL) {
        s->line = tmp;
    }
    else {
        s->line = g->stations[start]->line;
        l1 = s->line;
        while (l1->link != NULL) {
            if (l1->n == t) {
                flag = true;
                break;
            }
            l1 = l1->link;
        }
        // 같은 호선이 없다면
        if (flag == false) {
            l1->link = tmp;
        }
        else {
            flag = false;
        }
    }
    s->link = g->stations[start];
    g->stations[start] = s;
}

/* 최소값 선택 */
int choose(int dist[], int n, int found[]) {
    int i, min, minidx;
    min = INT_MAX;
    minidx = -1;
    for (i = 0; i < n; i++) {
        if (!found[i] && dist[i] < min) {
            min = dist[i];
            minidx = i;
        }
    }
    return minidx;
}

/* 소요시간 반환 */
int get_duration(Subway* s, int i) {
    Subway* tmp = s;
    while (tmp != NULL) {
        if (i == tmp->station) {
            return tmp->duration;
        }
        tmp = tmp->link;
    }
    return INF;
}

/* 경로 저장 */
void save_paths(int u) {
    PathType* tmp = pathlist;
    PathType* path = (PathType*)malloc(sizeof(PathType));
    path->station = u;
    path->link = NULL;

    if (pathlist == NULL) {
        pathlist = path;
    }
    else {
        while (tmp->link != NULL) {
            tmp = tmp->link;
        }
        tmp->link = path;
    }
}

/* 경로 삭제 */
PathType* delete_path(PathType* pre) {
    PathType* removed;
    removed = pre->link;
    pre->link = removed->link;
    free(removed);
    return pathlist;
}

/* 환승역 체크 후 불필요한 경로 삭제 */
void check_transfer(GraphType *g, int u) {
    bool flag = false;
    PathType* tmp = pathlist;

    for (int i = 0; i < MAX_TNF; i++) {
        if (tnf_num[i] == u) {
            flag = true;
            break;
        }
    }

    // 환승역이 아니라면 리턴
    if (flag == false) {
        return;
    }
        // 전 역의 호선과 같지 않은 경로를 삭제
    else {
        // 하나의 경로씩
        if (tmp != NULL) {
            while (tmp->link != NULL) {
                LineType* l = g->stations[tmp->link->station]->line;
                flag = false;
                // 호선들을 확인하여 같은 호선이 있는지 확인
                while (l != NULL) {
                    if (l->n == pretnf) {
                        flag = true;
                        break;
                    }
                }

                // 같은 호선이 없으므로 삭제해주어야함...
                if (flag == false) {
                    pathlist = delete_path(tmp);
                }

                tmp = tmp->link;
            }

            // 환승역 저장
            pretnf = u;
        }
    }
}


int selected[MAX_VERTICES];	// 최종경로저장
int max_size;
int line[MAX_VERTICES];

/* 최단경로 알고리즘 */
void shortest_path(GraphType* g, int start, int end) {
    int i, u, w;
    pathlist = nullptr;
    int idx = 0;
    StackType s[MAX_VERTICES];

    for (int i = 0; i < MAX_VERTICES; i++) {
        init_stack(&s[i]);
        selected[i] = INF;
    }

    for (i = 0; i < MAX_VERTICES; i++) {
        dist[i] = get_duration(g->stations[start], i);
        if (dist[i] != INF) {
            push(&s[i], start);
        }
        found[i] = FALSE;
    }
    found[start] = TRUE;
    dist[start] = 0;
    pretnf = start;	// error!!
    cout << stn_name[start] << " -> ";
    for (i = 0; i < MAX_VERTICES; i++) {
        u = choose(dist, MAX_VERTICES, found);

        //save_paths(u); // u를 리스트에 저장(거쳐간 정점들 저장)
        //check_transfer(g, u);


        // u가 환승역인지 확인 후
        // 전 호선과 비교하여 거쳐간 정점들을 저장한 리스트에서 환승역의 호선이 아닌 부분을 삭제

        pretnf = u;	// error!! 전 정점을 저장
        found[u] = TRUE;
        for (w = 0; w < MAX_VERTICES; w++) {
            if (!found[w] && (dist[u] + get_duration(g->stations[u], w) < dist[w])) {
                dist[w] = dist[u] + get_duration(g->stations[u], w);
                push(&s[w], u);
            }
        }
        if (end == u) break;
    }

    int ii = -1;
    selected[++ii] = end;
    selected[++ii] = pop(&s[end]);

    int j = ii;

    while (1) {
        if (selected[j] != INF) {
            j = ii;
            while (!is_empty_stack(&s[selected[j]])) {
                selected[++ii] = pop(&s[selected[j]]);
            }
        }
        if (selected[ii] == start) break;
    }

    max_size = ii;
    bool flag = false;
    int count = 0;

    for(int i=0; i<MAX_VERTICES; i++) {
        line[i] = 0;
    }

    for (ii;ii >= 0;ii--) {
        int tmp;
        flag = false;
        LineType* test1 = g->stations[selected[ii]]->line;

        if (ii != 0) {
            while (test1 != NULL) {

                LineType* test2 = g->stations[selected[ii - 1]]->line;

                while (test2 != NULL) {

                    if (test1->n == test2->n) {
                        tmp = test1->n;
                        cout << tmp << "호선";
                        line[count++] = tmp;
                        flag = true;
                        break;
                    }
                    test2 = test2->link;
                }
                test1 = test1->link;
                if (flag == true) break;
            }
        }
        else {
            cout << tmp << "호선";
            line[count++] = tmp;
        }

        cout << stn_name[selected[ii]] << endl;
        cout << endl;
    }

    flag = false;

    LineType* test3 = g->stations[selected[ii + 1]]->line;
    while (test3 != NULL) {
        LineType* test4 = g->stations[end]->line;
        while (test4 != NULL) {
            if (test3->n == test4->n) {
                cout << test3->n << "호선" << endl;
                line[count++] = test3->n;
                flag = true;
                break;
            }
            test4 = test4->link;
        }
        test3 = test3->link;
        if (flag == true) break;
    }

    cout << stn_name[end];
}

void print(GraphType* g) {
    LineType* l;
    for (int i = 0; i < MAX_VERTICES; i++) {
        l = g->stations[i]->line;
        cout << stn_name[g->stations[i]->station] << ": ";
        while (l != NULL) {
            cout << l->n << "호선 ";
            l = l->link;
        }
        cout << endl;
    }
}

extern "C" JNIEXPORT jintArray JNICALL
Java_com_example_subway_Subway_shortest_1path(
        JNIEnv *env,
        jobject /* this */,
        jstring start, jstring end, jintArray lineArray, jintArray totalArray) {

    const char* s = env->GetStringUTFChars(start, 0);
    const char* e = env->GetStringUTFChars(end, 0);

    string ss(s);
    string ee(e);

    int start_station, end_station;
    for(int i=0; i<MAX_VERTICES; i++) {
        if(stn_name[i] == ss) {
            start_station = i;
        }
        if(stn_name[i] == ee) {
            end_station = i;
        }
    }

    GraphType g;

    init_graph(&g);

    insert_edge(&g, 신창, 온양온천, 5, 1);
    insert_edge(&g, 온양온천, 배방, 5, 1);
    insert_edge(&g, 배방, 아산, 5, 1);
    insert_edge(&g, 아산, 쌍용, 5, 1);
    insert_edge(&g, 쌍용, 봉명, 5, 1);
    insert_edge(&g, 봉명, 천안, 5, 1);
    insert_edge(&g, 천안, 두정, 5, 1);
    insert_edge(&g, 두정, 직산, 5, 1);
    insert_edge(&g, 직산, 성환, 5, 1);
    insert_edge(&g, 성환, 평택, 5, 1);
    insert_edge(&g, 평택, 지제, 5, 1);
    insert_edge(&g, 지제, 서정리, 5, 1);
    insert_edge(&g, 서정리, 송탄, 5, 1);
    insert_edge(&g, 송탄, 진위, 5, 1);
    insert_edge(&g, 진위, 오산, 5, 1);
    insert_edge(&g, 오산, 오산대, 5, 1);
    insert_edge(&g, 오산대, 세마, 5, 1);
    insert_edge(&g, 세마, 병점, 5, 1);
    insert_edge(&g, 서동탄, 병점, 5, 1);
    insert_edge(&g, 병점, 세류, 5, 1);
    insert_edge(&g, 세류, 수원, 5, 1);
    insert_edge(&g, 수원, 화서, 5, 1);
    insert_edge(&g, 화서, 성균관대, 5, 1);
    insert_edge(&g, 성균관대, 의왕, 5, 1);
    insert_edge(&g, 의왕, 당정, 5, 1);
    insert_edge(&g, 당정, 군포, 5, 1);
    insert_edge(&g, 군포, 금정, 5, 1);
    insert_edge(&g, 금정, 명학, 5, 1);
    insert_edge(&g, 명학, 안양, 5, 1);
    insert_edge(&g, 안양, 관악, 5, 1);
    insert_edge(&g, 관악, 석수, 5, 1);
    insert_edge(&g, 석수, 금천구청, 5, 1);
    insert_edge(&g, 광명, 금천구청, 5, 1);
    insert_edge(&g, 금천구청, 독산, 5, 1);
    insert_edge(&g, 독산, 가산디지털단지, 5, 1);
    insert_edge(&g, 가산디지털단지, 구로, 5, 1);
    insert_edge(&g, 구로, 신도림, 5, 1);
    insert_edge(&g, 신도림, 영등포, 5, 1);
    insert_edge(&g, 영등포, 신길, 5, 1);
    insert_edge(&g, 신길, 대방, 5, 1);
    insert_edge(&g, 대방, 노량진, 5, 1);
    insert_edge(&g, 노량진, 용산, 5, 1);
    insert_edge(&g, 용산, 남영, 5, 1);
    insert_edge(&g, 남영, 서울역, 5, 1);
    insert_edge(&g, 서울역, 시청, 5, 1);
    insert_edge(&g, 시청, 종각, 5, 1);
    insert_edge(&g, 종각, 종로3가, 5, 1);
    insert_edge(&g, 종로3가, 종로5가, 5, 1);
    insert_edge(&g, 종로5가, 동대문, 5, 1);
    insert_edge(&g, 동대문, 동묘앞, 5, 1);
    insert_edge(&g, 동묘앞, 신설동, 5, 1);
    insert_edge(&g, 신설동, 제기동, 5, 1);
    insert_edge(&g, 제기동, 청량리, 5, 1);
    insert_edge(&g, 청량리, 회기, 5, 1);
    insert_edge(&g, 회기, 외대앞, 5, 1);
    insert_edge(&g, 외대앞, 신이문, 5, 1);
    insert_edge(&g, 신이문, 석계, 5, 1);
    insert_edge(&g, 석계, 광운대, 5, 1);
    insert_edge(&g, 광운대, 월계, 5, 1);
    insert_edge(&g, 월계, 녹천, 5, 1);
    insert_edge(&g, 녹천, 창동, 5, 1);
    insert_edge(&g, 창동, 방학, 5, 1);
    insert_edge(&g, 방학, 도봉, 5, 1);
    insert_edge(&g, 도봉, 도봉산, 5, 1);
    insert_edge(&g, 도봉산, 망월사, 5, 1);
    insert_edge(&g, 망월사, 회룡, 5, 1);
    insert_edge(&g, 회룡, 의정부, 5, 1);
    insert_edge(&g, 의정부, 가능, 5, 1);
    insert_edge(&g, 가능, 녹양, 5, 1);
    insert_edge(&g, 녹양, 양주, 5, 1);
    insert_edge(&g, 양주, 덕계, 5, 1);
    insert_edge(&g, 덕계, 덕정, 5, 1);
    insert_edge(&g, 덕정, 지행, 5, 1);
    insert_edge(&g, 지행, 동두천중앙, 5, 1);
    insert_edge(&g, 동두천중앙, 보산, 5, 1);
    insert_edge(&g, 보산, 동두천, 5, 1);
    insert_edge(&g, 동두천, 소요산, 5, 1);

    insert_edge(&g, 구로, 구일, 5, 1);
    insert_edge(&g, 구일, 개봉, 5, 1);
    insert_edge(&g, 개봉, 오류동, 5, 1);
    insert_edge(&g, 오류동, 온수, 5, 1);
    insert_edge(&g, 온수, 역곡, 5, 1);
    insert_edge(&g, 역곡, 소사, 5, 1);
    insert_edge(&g, 소사, 부천, 5, 1);
    insert_edge(&g, 부천, 중동, 5, 1);
    insert_edge(&g, 중동, 송내, 5, 1);
    insert_edge(&g, 송내, 부개, 5, 1);
    insert_edge(&g, 부개, 부평, 5, 1);
    insert_edge(&g, 부평, 백운, 5, 1);
    insert_edge(&g, 백운, 동암, 5, 1);
    insert_edge(&g, 동암, 간석, 5, 1);
    insert_edge(&g, 간석, 주안, 5, 1);
    insert_edge(&g, 주안, 도화, 5, 1);
    insert_edge(&g, 도화, 제물포, 5, 1);
    insert_edge(&g, 제물포, 도원, 5, 1);
    insert_edge(&g, 도원, 동인천, 5, 1);
    insert_edge(&g, 동인천, 인천, 5, 1);

    insert_edge(&g, 까치산, 신정네거리, 2, 2);
    insert_edge(&g, 신정네거리, 양천구청, 2, 2);
    insert_edge(&g, 양천구청, 도림천, 2, 2);
    insert_edge(&g, 도림천, 신도림, 2, 2);

    insert_edge(&g, 신도림, 문래, 2, 2);
    insert_edge(&g, 문래, 영등포구청, 2, 2);
    insert_edge(&g, 영등포구청, 당산, 2, 2);
    insert_edge(&g, 당산, 합정, 2, 2);
    insert_edge(&g, 합정, 홍대입구, 2, 2);
    insert_edge(&g, 홍대입구, 신촌, 2, 2);
    insert_edge(&g, 신촌, 이대, 2, 2);
    insert_edge(&g, 이대, 아현, 2, 2);
    insert_edge(&g, 아현, 충정로, 2, 2);
    insert_edge(&g, 충정로, 시청, 2, 2);
    insert_edge(&g, 시청, 을지로입구, 2, 2);
    insert_edge(&g, 을지로입구, 을지로3가, 2, 2);
    insert_edge(&g, 을지로3가, 을지로4가, 2, 2);
    insert_edge(&g, 을지로4가, 동대문역사문화공원, 2, 2);
    insert_edge(&g, 동대문역사문화공원, 신당, 2, 2);
    insert_edge(&g, 신당, 상왕십리, 2, 2);
    insert_edge(&g, 상왕십리, 왕십리, 2, 2);
    insert_edge(&g, 왕십리, 한양대, 2, 2);
    insert_edge(&g, 한양대, 뚝섬, 2, 2);
    insert_edge(&g, 뚝섬, 성수, 2, 2);
    insert_edge(&g, 성수, 건대입구, 2, 2);

    insert_edge(&g, 성수, 용답, 2, 2);
    insert_edge(&g, 용답, 신답, 2, 2);
    insert_edge(&g, 신답, 용두, 2, 2);
    insert_edge(&g, 용두, 신설동, 2, 2);

    insert_edge(&g, 건대입구, 구의, 2, 2);
    insert_edge(&g, 구의, 강변, 2, 2);
    insert_edge(&g, 강변, 잠실나루, 2, 2);
    insert_edge(&g, 잠실나루, 잠실, 2, 2);
    insert_edge(&g, 잠실, 잠실새내, 2, 2);
    insert_edge(&g, 잠실새내, 종합운동장, 2, 2);
    insert_edge(&g, 종합운동장, 삼성, 2, 2);
    insert_edge(&g, 삼성, 선릉, 2, 2);
    insert_edge(&g, 선릉, 역삼, 2, 2);
    insert_edge(&g, 역삼, 강남, 2, 2);
    insert_edge(&g, 강남, 교대, 2, 2);
    insert_edge(&g, 교대, 서초, 2, 2);
    insert_edge(&g, 서초, 방배, 2, 2);
    insert_edge(&g, 방배, 사당, 2, 2);
    insert_edge(&g, 사당, 낙성대, 2, 2);
    insert_edge(&g, 낙성대, 서울대입구, 2, 2);
    insert_edge(&g, 서울대입구, 봉천, 2, 2);
    insert_edge(&g, 봉천, 신림, 2, 2);
    insert_edge(&g, 신림, 신대방, 2, 2);
    insert_edge(&g, 신대방, 구로디지털단지, 2, 2);
    insert_edge(&g, 구로디지털단지, 대림, 2, 2);
    insert_edge(&g, 대림, 신도림, 2, 2);

    insert_edge(&g, 오금  , 경찰병원, 4, 3);
    insert_edge(&g, 경찰병원, 가락시장, 4, 3);
    insert_edge(&g, 가락시장, 수서, 4, 3);
    insert_edge(&g, 수서, 일원, 4, 3);
    insert_edge(&g, 일원, 대청, 4, 3);
    insert_edge(&g, 대청, 학여울, 4, 3);
    insert_edge(&g, 학여울, 대치, 4, 3);
    insert_edge(&g, 대치, 도곡, 4, 3);
    insert_edge(&g, 도곡, 매봉, 4, 3);
    insert_edge(&g, 매봉, 양재, 4, 3);
    insert_edge(&g, 양재, 남부터미널, 4, 3);
    insert_edge(&g, 남부터미널, 교대, 4, 3);
    insert_edge(&g, 교대, 고속터미널, 4, 3);
    insert_edge(&g, 고속터미널, 잠원, 4, 3);
    insert_edge(&g, 잠원, 신사, 4, 3);
    insert_edge(&g, 신사, 압구정, 4, 3);
    insert_edge(&g, 압구정, 옥수, 4, 3);
    insert_edge(&g, 옥수, 금호, 4, 3);
    insert_edge(&g, 금호, 약수, 4, 3);
    insert_edge(&g, 약수, 동대입구, 4, 3);
    insert_edge(&g, 동대입구, 충무로, 4, 3);
    insert_edge(&g, 충무로, 을지로3가, 4, 3);
    insert_edge(&g, 을지로3가, 종로3가, 4, 3);
    insert_edge(&g, 종로3가, 안국, 4, 3);
    insert_edge(&g, 안국, 경복궁, 4, 3);
    insert_edge(&g, 경복궁, 독립문, 4, 3);
    insert_edge(&g, 독립문, 무악재, 4, 3);
    insert_edge(&g, 무악재, 홍제, 4, 3);
    insert_edge(&g, 홍제, 녹번, 4, 3);
    insert_edge(&g, 녹번, 불광, 4, 3);
    insert_edge(&g, 불광, 연신내, 4, 3);
    insert_edge(&g, 연신내, 구파발, 4, 3);
    insert_edge(&g, 구파발, 지축, 4, 3);
    insert_edge(&g, 지축, 삼송, 4, 3);
    insert_edge(&g, 삼송, 원흥, 4, 3);
    insert_edge(&g, 원흥, 원당, 4, 3);
    insert_edge(&g, 원당, 화정, 4, 3);
    insert_edge(&g, 화정, 대곡, 4, 3);
    insert_edge(&g, 대곡, 백석, 4, 3);
    insert_edge(&g, 백석, 마두, 4, 3);
    insert_edge(&g, 마두, 정발산, 4, 3);
    insert_edge(&g, 정발산, 주엽, 4, 3);
    insert_edge(&g, 주엽, 대화, 4, 3);


    insert_edge(&g, 오이도 ,  정왕, 3, 4);
    insert_edge(&g, 정왕, 신길온천, 3, 4);
    insert_edge(&g, 신길온천, 안산, 3, 4);
    insert_edge(&g, 안산, 초지, 3, 4);
    insert_edge(&g, 초지, 고잔, 3, 4);
    insert_edge(&g, 고잔, 중앙, 3, 4);
    insert_edge(&g, 중앙, 한대앞, 3, 4);
    insert_edge(&g, 한대앞, 상록수, 3, 4);
    insert_edge(&g, 상록수, 반월, 3, 4);
    insert_edge(&g, 반월, 대야미, 3, 4);
    insert_edge(&g, 대야미, 수리산, 3, 4);
    insert_edge(&g, 수리산, 산본, 3, 4);
    insert_edge(&g, 산본, 금정, 3, 4);
    insert_edge(&g, 금정, 범계, 3, 4);
    insert_edge(&g, 범계, 평촌, 3, 4);
    insert_edge(&g, 평촌, 인덕원, 3, 4);
    insert_edge(&g, 인덕원, 정부과천청사, 3, 4);
    insert_edge(&g, 정부과천청사, 과천, 3, 4);
    insert_edge(&g, 과천, 대공원, 3, 4);
    insert_edge(&g, 대공원, 경마공원, 3, 4);
    insert_edge(&g, 경마공원, 선바위, 3, 4);
    insert_edge(&g, 선바위, 남태령, 3, 4);
    insert_edge(&g, 남태령, 사당, 3, 4);
    insert_edge(&g, 사당, 총신대입구, 3, 4);
    insert_edge(&g, 총신대입구, 동작, 3, 4);
    insert_edge(&g, 동작, 이촌, 3, 4);
    insert_edge(&g, 이촌, 신용산, 3, 4);
    insert_edge(&g, 신용산, 삼각지, 3, 4);
    insert_edge(&g, 삼각지, 숙대입구, 3, 4);
    insert_edge(&g, 숙대입구, 서울역, 3, 4);
    insert_edge(&g, 서울역, 회현, 3, 4);
    insert_edge(&g, 회현, 명동, 3, 4);
    insert_edge(&g, 명동, 충무로, 3, 4);
    insert_edge(&g, 충무로, 동대문역사문화공원, 3, 4);
    insert_edge(&g, 동대문역사문화공원, 동대문, 3, 4);
    insert_edge(&g, 동대문, 혜화, 3, 4);
    insert_edge(&g, 혜화, 한성대입구, 3, 4);
    insert_edge(&g, 한성대입구, 성신여대입구, 3, 4);
    insert_edge(&g, 성신여대입구, 길음, 3, 4);
    insert_edge(&g, 길음, 미아사거리, 3, 4);
    insert_edge(&g, 미아사거리, 미아, 3, 4);
    insert_edge(&g, 미아, 수유, 3, 4);
    insert_edge(&g, 수유, 쌍문, 3, 4);
    insert_edge(&g, 쌍문, 창동, 3, 4);
    insert_edge(&g, 창동, 노원, 3, 4);
    insert_edge(&g, 노원, 상계, 3, 4);
    insert_edge(&g, 상계, 당고개, 3, 4);

    insert_edge(&g, 방화, 개화산, 4, 5);
    insert_edge(&g, 개화산, 김포공항, 4, 5);
    insert_edge(&g, 김포공항, 송정, 4, 5);
    insert_edge(&g, 송정, 마곡, 4, 5);
    insert_edge(&g, 마곡, 발산, 4, 5);
    insert_edge(&g, 발산, 우장산, 4, 5);
    insert_edge(&g, 우장산, 화곡, 4, 5);
    insert_edge(&g, 화곡, 까치산, 4, 5);
    insert_edge(&g, 까치산, 신정, 4, 5);
    insert_edge(&g, 신정, 목동, 4, 5);
    insert_edge(&g, 목동, 오목교, 4, 5);
    insert_edge(&g, 오목교, 양평, 4, 5);
    insert_edge(&g, 양평, 영등포구청, 4, 5);
    insert_edge(&g, 영등포구청, 영등포시장, 4, 5);
    insert_edge(&g, 영등포시장, 신길, 4, 5);
    insert_edge(&g, 신길, 여의도, 4, 5);
    insert_edge(&g, 여의도, 여의나루, 4, 5);
    insert_edge(&g, 여의나루, 마포, 4, 5);
    insert_edge(&g, 마포, 공덕, 4, 5);
    insert_edge(&g, 공덕, 애오개, 4, 5);
    insert_edge(&g, 애오개, 충정로, 4, 5);
    insert_edge(&g, 충정로, 서대문, 4, 5);
    insert_edge(&g, 서대문, 광화문, 4, 5);
    insert_edge(&g, 광화문, 종로3가, 4, 5);
    insert_edge(&g, 종로3가, 을지로4가, 4, 5);
    insert_edge(&g, 을지로4가, 동대문역사문화공원, 4, 5);
    insert_edge(&g, 동대문역사문화공원, 청구, 4, 5);
    insert_edge(&g, 청구, 신금호, 4, 5);
    insert_edge(&g, 신금호, 행당, 4, 5);
    insert_edge(&g, 행당, 왕십리, 4, 5);
    insert_edge(&g, 왕십리, 마장, 4, 5);
    insert_edge(&g, 마장, 답십리, 4, 5);
    insert_edge(&g, 답십리, 장한평, 4, 5);
    insert_edge(&g, 장한평, 군자, 4, 5);
    insert_edge(&g, 군자, 아차산, 4, 5);
    insert_edge(&g, 아차산, 광나루, 4, 5);
    insert_edge(&g, 광나루, 천호, 4, 5);
    insert_edge(&g, 천호, 강동, 4, 5);
    insert_edge(&g, 강동, 둔촌동, 4, 5);
    insert_edge(&g, 둔촌동, 올림픽공원, 4, 5);
    insert_edge(&g, 올림픽공원, 방이, 4, 5);
    insert_edge(&g, 방이, 오금, 4, 5);
    insert_edge(&g, 오금, 개롱, 4, 5);
    insert_edge(&g, 개롱, 거여, 4, 5);
    insert_edge(&g, 거여, 마천, 4, 5);
    insert_edge(&g, 강동, 길동, 4, 5);
    insert_edge(&g, 길동, 굽은다리, 4, 5);
    insert_edge(&g, 굽은다리, 명일, 4, 5);
    insert_edge(&g, 명일, 고덕, 4, 5);
    insert_edge(&g, 고덕, 상일동, 4, 5);

    insert_edge(&g, 봉화산, 화랑대, 3, 6);
    insert_edge(&g, 화랑대, 태릉입구, 3, 6);
    insert_edge(&g, 태릉입구, 석계, 4, 6);
    insert_edge(&g, 석계, 돌곶이, 3, 6);
    insert_edge(&g, 돌곶이, 상월곡, 3, 6);
    insert_edge(&g, 상월곡, 월곡, 4, 6);
    insert_edge(&g, 월곡, 고려대, 3, 6);
    insert_edge(&g, 고려대, 안암, 3, 6);
    insert_edge(&g, 안암, 보문, 4, 6);
    insert_edge(&g, 보문, 창신, 3, 6);
    insert_edge(&g, 창신, 동묘앞, 3, 6);
    insert_edge(&g, 동묘앞, 신당, 4, 6);
    insert_edge(&g, 신당, 청구, 3, 6);
    insert_edge(&g, 청구, 약수, 3, 6);
    insert_edge(&g, 약수, 버티고개, 4, 6);
    insert_edge(&g, 버티고개, 한강진, 3, 6);
    insert_edge(&g, 한강진, 이태원, 3, 6);
    insert_edge(&g, 이태원, 녹사평, 4, 6);
    insert_edge(&g, 녹사평, 삼각지, 3, 6);
    insert_edge(&g, 삼각지, 효창공원앞, 3, 6);
    insert_edge(&g, 효창공원앞, 공덕, 4, 6);
    insert_edge(&g, 공덕, 대흥, 3, 6);
    insert_edge(&g, 대흥, 광흥창, 3, 6);
    insert_edge(&g, 광흥창, 상수, 4, 6);
    insert_edge(&g, 상수, 합정, 3, 6);
    insert_edge(&g, 합정, 망원, 3, 6);
    insert_edge(&g, 망원, 마포구청, 4, 6);
    insert_edge(&g, 마포구청, 월드컵경기장, 3, 6);
    insert_edge(&g, 월드컵경기장, 디지털미디어시티, 3, 6);
    insert_edge(&g, 디지털미디어시티, 증산, 4, 6);
    insert_edge(&g, 증산, 새절, 3, 6);
    insert_edge(&g, 새절, 응암, 3, 6);

    /* 단방향 */
    insert_edge_oneway(&g, 응암, 역촌, 4, 6);
    insert_edge_oneway(&g, 역촌, 불광, 3, 6);
    insert_edge_oneway(&g, 불광, 독바위, 3, 6);
    insert_edge_oneway(&g, 독바위, 연신내, 4, 6);
    insert_edge_oneway(&g, 연신내, 구산, 3, 6);
    insert_edge_oneway(&g, 구산, 응암, 3, 6);

    insert_edge(&g, 부평구청, 굴포천, 4, 7);
    insert_edge(&g, 굴포천, 삼산체육관, 4, 7);
    insert_edge(&g, 삼산체육관, 상동, 4, 7);
    insert_edge(&g, 상동, 부천시청, 4, 7);
    insert_edge(&g, 부천시청, 신중동, 3, 7);
    insert_edge(&g, 신중동, 춘의, 4, 7);
    insert_edge(&g, 춘의, 부천종합운동장, 4, 7);
    insert_edge(&g, 부천종합운동장, 까치울, 4, 7);
    insert_edge(&g, 까치울, 온수, 4, 7);
    insert_edge(&g, 온수, 천왕, 3, 7);
    insert_edge(&g, 천왕, 광명사거리, 4, 7);
    insert_edge(&g, 광명사거리, 철산, 4, 7);
    insert_edge(&g, 철산, 가산디지털단지, 4, 7);
    insert_edge(&g, 가산디지털단지, 남구로, 4, 7);
    insert_edge(&g, 남구로, 대림, 3, 7);
    insert_edge(&g, 대림, 신풍, 4, 7);
    insert_edge(&g, 신풍, 보라매, 4, 7);
    insert_edge(&g, 보라매, 신대방삼거리, 4, 7);
    insert_edge(&g, 신대방삼거리, 장승배기, 4, 7);
    insert_edge(&g, 장승배기, 상도, 3, 7);
    insert_edge(&g, 상도, 숭실대입구, 4, 7);
    insert_edge(&g, 숭실대입구, 남성, 4, 7);
    insert_edge(&g, 남성, 총신대입구, 4, 7);
    insert_edge(&g, 총신대입구, 내방, 4, 7);
    insert_edge(&g, 내방, 고속터미널, 3, 7);
    insert_edge(&g, 고속터미널, 반포, 4, 7);
    insert_edge(&g, 반포, 논현, 4, 7);
    insert_edge(&g, 논현, 학동, 4, 7);
    insert_edge(&g, 학동, 강남구청, 4, 7);
    insert_edge(&g, 강남구청, 청담, 3, 7);
    insert_edge(&g, 청담, 뚝섬유원지, 4, 7);
    insert_edge(&g, 뚝섬유원지, 건대입구, 4, 7);
    insert_edge(&g, 건대입구, 어린이대공원, 4, 7);
    insert_edge(&g, 어린이대공원, 군자, 4, 7);
    insert_edge(&g, 군자, 중곡, 3, 7);
    insert_edge(&g, 중곡, 용마산, 4, 7);
    insert_edge(&g, 용마산, 사가정, 4, 7);
    insert_edge(&g, 사가정, 면목, 4, 7);
    insert_edge(&g, 면목, 상봉, 4, 7);
    insert_edge(&g, 상봉, 중화, 3, 7);
    insert_edge(&g, 중화, 먹골, 4, 7);
    insert_edge(&g, 먹골, 태릉입구, 4, 7);
    insert_edge(&g, 태릉입구, 공릉, 4, 7);
    insert_edge(&g, 공릉, 하계, 4, 7);
    insert_edge(&g, 하계, 중계, 3, 7);
    insert_edge(&g, 중계, 노원, 4, 7);
    insert_edge(&g, 노원, 마들, 4, 7);
    insert_edge(&g, 마들, 수락산, 4, 7);
    insert_edge(&g, 수락산, 도봉산, 4, 7);
    insert_edge(&g, 도봉산, 장암, 3, 7);

    insert_edge(&g, 모란 ,   수진, 3, 8);
    insert_edge(&g, 수진, 신흥, 3, 8);
    insert_edge(&g, 신흥, 단대오거리, 3, 8);
    insert_edge(&g, 단대오거리, 남한산성입구, 3, 8);
    insert_edge(&g, 남한산성입구, 산성, 3, 8);
    insert_edge(&g, 산성, 복정, 3, 8);
    insert_edge(&g, 복정, 장지, 3, 8);
    insert_edge(&g, 장지, 문정, 3, 8);
    insert_edge(&g, 문정, 가락시장, 3, 8);
    insert_edge(&g, 가락시장, 송파, 3, 8);
    insert_edge(&g, 송파, 석촌, 3, 8);
    insert_edge(&g, 석촌, 잠실, 3, 8);
    insert_edge(&g, 잠실, 몽촌토성, 3, 8);
    insert_edge(&g, 몽촌토성, 강동구청, 3, 8);
    insert_edge(&g, 강동구청, 천호, 3, 8);
    insert_edge(&g, 천호, 암사, 3, 8);

    insert_edge(&g, 개화, 김포공항, 4, 9);
    insert_edge(&g, 김포공항, 공항시장, 4, 9);
    insert_edge(&g, 공항시장, 신방화, 4, 9);
    insert_edge(&g, 신방화, 마곡나루, 4, 9);
    insert_edge(&g, 마곡나루, 양천향교, 4, 9);
    insert_edge(&g, 양천향교, 가양, 4, 9);
    insert_edge(&g, 가양, 증미, 4, 9);
    insert_edge(&g, 증미, 등촌, 4, 9);
    insert_edge(&g, 등촌, 염창, 4, 9);
    insert_edge(&g, 염창, 신목동, 4, 9);
    insert_edge(&g, 신목동, 선유도, 4, 9);
    insert_edge(&g, 선유도, 당산, 4, 9);
    insert_edge(&g, 당산, 국회의사당, 4, 9);
    insert_edge(&g, 국회의사당, 여의도, 4, 9);
    insert_edge(&g, 여의도, 샛강, 4, 9);
    insert_edge(&g, 샛강, 노량진, 4, 9);
    insert_edge(&g, 노량진, 노들, 4, 9);
    insert_edge(&g, 노들, 흑석, 4, 9);
    insert_edge(&g, 흑석, 동작, 4, 9);
    insert_edge(&g, 동작, 구반포, 4, 9);
    insert_edge(&g, 구반포, 신반포, 4, 9);
    insert_edge(&g, 신반포, 고속터미널, 4, 9);
    insert_edge(&g, 고속터미널, 사평, 4, 9);
    insert_edge(&g, 사평, 신논현, 4, 9);
    insert_edge(&g, 신논현, 언주, 4, 9);
    insert_edge(&g, 언주, 선정릉, 4, 9);
    insert_edge(&g, 선정릉, 삼성중앙, 4, 9);
    insert_edge(&g, 삼성중앙, 봉은사, 4, 9);
    insert_edge(&g, 봉은사, 종합운동장, 4, 9);
    insert_edge(&g, 종합운동장, 삼전, 4, 9);
    insert_edge(&g, 삼전, 석촌고분, 4, 9);
    insert_edge(&g, 석촌고분, 석촌, 4, 9);
    insert_edge(&g, 석촌, 송파나루, 4, 9);
    insert_edge(&g, 송파나루, 한성백제, 4, 9);
    insert_edge(&g, 한성백제, 올림픽공원, 4, 9);
    insert_edge(&g, 올림픽공원, 둔촌오륜, 4, 9);
    insert_edge(&g, 둔촌오륜, 중앙보훈병원, 4, 9);

    shortest_path(&g, start_station, end_station);

    jint * distPointer = new jint[MAX_VERTICES];
    for(int i=0; i<MAX_VERTICES; i++) {
        distPointer[i] = dist[i];
    }
    env->SetIntArrayRegion(totalArray, 0, MAX_VERTICES, distPointer);

    jintArray result = env->NewIntArray(max_size+1);
    if(result == NULL) {
        return NULL;
    }

    jint * linePointer = new jint[max_size+1];
    for(int i=0; i<max_size+1; i++) {
        linePointer[i] = line[i];
    }

    env->SetIntArrayRegion(lineArray, 0, max_size+1, linePointer);

    jint * cintArray = new jint[max_size+1];
    for(int i=0; i<max_size+1; i++) {
        cintArray[i] = selected[i];
    }

    env->SetIntArrayRegion(result, 0, max_size+1, cintArray);

    delete[] cintArray;

    return result;
}

/*
extern "C" JNIEXPORT jint JNICALL
Java_com_example_subway_Subway_get_1total(
    JNIEnv *env,
    jobject, jint end) {
    return dist[end];
}
*/