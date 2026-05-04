import java.util.*;

public class Solution {
    static int[] parent;
    static boolean[][] usedEdgesMat;
    static boolean[] hasEdge;

    static int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }

    static void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) parent[root_i] = root_j;
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        if (!sc.hasNextInt()) return;
        int N = sc.nextInt();

        int P = sc.nextInt();
        int[] L = new int[P];
        for (int i = 0; i < P; i++) L[i] = sc.nextInt();

        int Q = sc.nextInt();
        int[] R = new int[Q];
        for (int i = 0; i < Q; i++) R[i] = sc.nextInt();

        if (N % 2 == 0) {
            if (N > 2) {
                System.out.println("N");
            } else {
                // N = 2, S = 2
                if (P + Q > 2) System.out.println("N");
                else if (P >= 2 && L[0] == L[1]) System.out.println("N");
                else if (Q >= 2 && R[0] == R[1]) System.out.println("N");
                else if (P >= 1 && Q >= 1 && L[0] == R[0]) System.out.println("N");
                else System.out.println("Y");
            }
            return;
        }

        long S = (long) N * (N - 1) / 2 + 1;
        if (P + Q > S) {
            System.out.println("N");
            return;
        }

        for (int i = 0; i < P - 1; i++) if (L[i] == L[i + 1]) { System.out.println("N"); return; }
        for (int i = 0; i < Q - 1; i++) if (R[i] == R[i + 1]) { System.out.println("N"); return; }

        if (P > 0 && Q > 0 && L[0] != R[Q - 1]) {
            System.out.println("N");
            return;
        }

        usedEdgesMat = new boolean[N + 1][N + 1];
        int usedCount = 0;
        for (int i = 0; i < P - 1; i++) {
            int u = L[i], v = L[i + 1];
            if (u > v) { int tmp = u; u = v; v = tmp; }
            if (usedEdgesMat[u][v]) { System.out.println("N"); return; }
            usedEdgesMat[u][v] = true;
            usedCount++;
        }
        for (int i = 0; i < Q - 1; i++) {
            int u = R[i], v = R[i + 1];
            if (u > v) { int tmp = u; u = v; v = tmp; }
            if (usedEdgesMat[u][v]) { System.out.println("N"); return; }
            usedEdgesMat[u][v] = true;
            usedCount++;
        }

        long E_prime = (S - 1) - usedCount;
        int u_target, v_target;
        if (P > 0 && Q > 0) { u_target = L[P - 1]; v_target = R[0]; }
        else if (P > 0) { u_target = L[P - 1]; v_target = L[0]; }
        else if (Q > 0) { u_target = R[Q - 1]; v_target = R[0]; }
        else { u_target = 1; v_target = 1; }

        if (E_prime == 0) {
            System.out.println(u_target == v_target ? "Y" : "N");
            return;
        }

        parent = new int[N + 1];
        hasEdge = new boolean[N + 1];
        for (int i = 1; i <= N; i++) parent[i] = i;
        for (int i = 1; i <= N; i++) {
            for (int j = i + 1; j <= N; j++) {
                if (!usedEdgesMat[i][j]) {
                    unite(i, j);
                    hasEdge[i] = hasEdge[j] = true;
                }
            }
        }

        int edgeComp = -1;
        for (int i = 1; i <= N; i++) {
            if (hasEdge[i]) {
                int c = find(i);
                if (edgeComp == -1) edgeComp = c;
                else if (edgeComp != c) { System.out.println("N"); return; }
            }
        }

        if (edgeComp != -1) {
            if (find(u_target) != edgeComp || find(v_target) != edgeComp) {
                System.out.println("N");
                return;
            }
        }

        System.out.println("Y");
    }
}
