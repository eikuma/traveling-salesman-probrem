#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define INF 10000000
#define n 6 //都市数
#define TRUE 1
#define FALSE 0

int distance[n][n] = {0}; //都市間の距離

/*-------------------------最近傍法---------------------------*/
void nearest_neighbor(); //最近傍法
//https://withcation.com/2019/05/23/post-526
/*-----------------------------------------------------------*/

/*---------------------------貪欲法--------------------------*/
void greedy();            //貪欲法
int root(int x);          //木の根を求める
int same(int x, int y);   //xとyが同じ集合に属するか判定
void unite(int x, int y); //xとyの属する集合を併合
int par[n];               //根を記録
int G[n][n] = {FALSE};    //通った経路を保存
// https://www.slideshare.net/chokudai/union-find-49066733
//http://www.nct9.ne.jp/m_hiroi/light/pyalgo62.html
/*----------------------------------------------------------*/

/*----------------------2重最小全域木法---------------------*/
void double_MST();          //2重最小全域木法
void Prim();                //2重最小全域木を作成
void DFS(int v);            //深さ優先探索
int T[n][n] = {FALSE};      //2重最小全域木
int flag[n] = {FALSE};      //深さ優先探索で訪れたかどうか
int order[2 * n] = {FALSE}; //探索時に訪れた順番を保存
int count = 0;
#define state1 1 //T(最小全域木に含まれた頂点の集合)に属する
#define state2 2 //Tに属する頂点に隣接する
#define state3 3 //それ以外
//https://programgenjin.hatenablog.com/entry/2019/03/19/101827
/*----------------------------------------------------------*/

int main()
{
    unsigned int seed;

    /* 現在時刻を元に seed を決定(別の方法もある) */
    seed = (unsigned)time(NULL);
    srand(seed);

    // 都市間の距離行列を作成
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            int tmp = rand() % 9 + 1;
            distance[i][j] = tmp;
            distance[j][i] = tmp;
        }
    }

    // 都市間の距離行列を表示
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%d ", distance[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    clock_t start, end;
    // start = clock();
    nearest_neighbor();
    // end = clock();
    // printf("%.2f秒かかりました\n", (double)(end - start) / CLOCKS_PER_SEC);
    // start = clock();
    greedy();
    // end = clock();
    // printf("%.2f秒かかりました\n", (double)(end - start) / CLOCKS_PER_SEC);
    // start = clock();
    double_MST();
    // end = clock();
    // printf("%.2f秒かかりました\n", (double)(end - start) / CLOCKS_PER_SEC);
}

/*----------------------------------------------------------*/
void nearest_neighbor()
{
    int i, j, length = 0;
    int first = 0;       //最初に訪れる都市
    int visited[n];      //訪れたかどうか
    int current = first; //現在の都市
    int next[n];         //次の都市を記録

    //visitedを初期化
    for (i = 0; i < n; i++)
    {
        visited[i] = FALSE;
    }

    //最初に訪れる都市に，訪れたフラッグを立てる
    visited[first] = TRUE;

    //実行
    for (i = 1; i < n; i++)
    {
        int min = INF;
        for (j = 0; j < n; j++) //j地点から最小の距離の地点を探す
        {
            if (visited[j] == FALSE) //まだ訪れていなかったら
            {
                if (distance[current][j] < min)
                {
                    min = distance[current][j];
                    next[current] = j;
                }
            }
        }
        current = next[current]; //現在地点を更新
        visited[current] = TRUE; //訪れたフラッグを立てる

        length += min;
    }
    length += distance[current][first];

    // 最初の都市に戻る
    next[current] = first;

    //出力
    printf("最近傍法\n");
    for (i = 0, current = first; i < n; i++, current = next[current])
    {
        printf("%d → ", current);
    }
    printf("%d", first);
    printf("\nLength:%d\n\n", length);
}
/*----------------------------------------------------------*/

void greedy()
{
    int i, j, k, c;
    int from, to; //都市を一時保存する変数
    int length = 0;
    int edge[n] = {0}; //都市から出る辺の本数をカウント

    //parを全部の頂点が根になるように初期化
    for (i = 0; i < n; i++)
        par[i] = i;

    //実行
    for (k = 0; k < n; k++)
    {
        int min = INF;

        // 最も短い辺を探索
        for (i = 0; i < n - 1; i++)
        {
            for (j = i + 1; j < n; j++)
            {
                if ((distance[i][j] < min) && (edge[i] < 2 && edge[j] < 2) && (same(i, j) == FALSE || k == n - 1))
                {
                    //i地点とj地点の距離がminより小さい
                    // i地点とj地点から出る辺が2未満
                    // i地点とj地点が同じ集合じゃない，または最後の辺

                    min = distance[i][j]; //minを更新
                    // i地点とj地点のを一時保存
                    from = i;
                    to = j;
                }
            }
        }
        length += min; //距離を加算
        //辺の本数を加算
        edge[from]++;
        edge[to]++;
        //from->toとto->fromをGに追加
        G[from][to]++;
        G[to][from]++;

        unite(from, to); //fromとtoの集合を併合
    }

    //出力
    printf("貪欲法\n");
    for (i = 0, c = 0; c < n; c++)
    {
        for (j = 0; j < n; j++)
        {
            if (G[i][j] == 1)
            {
                printf("%d → ", i);
                G[i][j] = 0;
                G[j][i] = 0;
                i = j;
                break;
            }
        }
    }
    printf("0");
    printf("\nLength:%d\n\n", length);
}

//木の根を求める
int root(int x)
{
    if (par[x] == x)
        return x; //根
    else
        return par[x] = root(par[x]); //経路圧縮
}

//xとyが同じ集合に属するか否か
int same(int x, int y)
{
    x = root(x);
    y = root(y);
    return x == y ? TRUE : FALSE;
}

//xとyの属する集合を合併
void unite(int x, int y)
{
    x = root(x);
    y = root(y);
    if (x == y)
        return;

    par[x] = y;
}

/*----------------------------------------------------------*/
void double_MST()
{
    int i, j, c, length = 0;
    int order_count[n] = {0}; //探索時に訪れた回数を計算
    Prim();                   //2重最小全域木を作成
    DFS(0);                   //0地点から深さ優先探索を開始

    //辺を短絡
    for (i = 0; i < count - 1; i++)
    {
        order_count[order[i]]++;
        if (order_count[order[i]] > 1)
        {
            if (T[order[i]][order[i - 1]] != 0)
            {
                T[order[i - 1]][order[i]]--;
                T[order[i]][order[i - 1]]--;
                T[order[i]][order[i + 1]]--;
                T[order[i + 1]][order[i]]--;
                T[order[i - 1]][order[i + 1]]++;
                T[order[i + 1]][order[i - 1]]++;
            }
            else
            {
                T[order[i - 2]][order[i]]--;
                T[order[i]][order[i - 2]]--;
                T[order[i]][order[i + 1]]--;
                T[order[i + 1]][order[i]]--;
                T[order[i - 2]][order[i + 1]]++;
                T[order[i + 1]][order[i - 2]]++;
            }
        }
    }

    //出力
    printf("2重最小全域木法\n");
    for (i = 0, c = 0; c < n; c++)
    {
        for (j = 0; j < n; j++)
        {
            if (T[i][j] == 1)
            {
                // 距離を計算
                length += distance[i][j];
                printf("%d → ", i);
                T[i][j] = 0;
                T[j][i] = 0;
                i = j;
                break;
            }
        }
    }
    length += distance[i][0];
    printf("0");
    printf("\nLength:%d\n", length);
}

//2重最小全域木を作成
void Prim()
{
    int i, vertex, min_cost;

    struct point
    {
        //都市の情報を保存する構造体
        int cost;
        int state;
        int from;
        int to;
    };

    struct point point[n];

    // 初期化
    for (i = 0; i < n; i++)
    {
        point[i].cost = INF;
        point[i].state = state3;
        point[i].from = 0;
        point[i].to = 0;
    }
    point[0].cost = 0;
    point[0].state = state2;

    while (1)
    {
        // Tに隣接する頂点の中で最小のコストを持つものを選ぶ
        vertex = -1;
        min_cost = INF;
        for (i = 0; i < n; i++)
        {
            if (point[i].state == state2 && point[i].cost < min_cost)
            {
                min_cost = point[i].cost;
                vertex = i;
            }
        }

        // T = V(グラフに属する全ての頂点の集合) ならば終了
        if (vertex == -1)
            break;

        point[vertex].state = state1; // vertexをTに加える

        if (point[vertex].from != point[vertex].to)
        { //最小全域木に追加
            T[point[vertex].from][point[vertex].to] = 2;
            T[point[vertex].to][point[vertex].from] = 2;
        }

        // vertexに隣接し、Tに属していない頂点のコストを更新
        for (i = 0; i < n; i++)
        {
            if (point[i].state != state1)
            {
                point[i].state = state2;
                if (point[i].cost > distance[vertex][i])
                {
                    point[i].cost = distance[vertex][i];
                    point[i].from = vertex;
                    point[i].to = i;
                }
            }
        }
    }
}

// 深さ優先探索
void DFS(int v)
{
    order[count++] = v;
    flag[v] = TRUE;

    for (int i = 0; i < n; i++)
    {
        if (flag[i] == FALSE && T[v][i] > 0)
        {
            DFS(i);
            order[count++] = v;
        }
    }
    return;
}
/*----------------------------------------------------------*/
