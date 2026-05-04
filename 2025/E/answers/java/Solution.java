import java.util.*;
import java.io.*;

public class Solution {
    static final long INF = 1L << 50;

    static class Node {
        long minVal = INF;
        long lazy = 0;
    }

    static Node[] tree;
    static int nTree;

    static void push(int node) {
        if (tree[node].lazy != 0) {
            long lz = tree[node].lazy;
            tree[2 * node].minVal += lz;
            tree[2 * node].lazy += lz;
            tree[2 * node + 1].minVal += lz;
            tree[2 * node + 1].lazy += lz;
            tree[node].lazy = 0;
        }
    }

    static void updateRange(int node, int l, int r, int ql, int qr, int val) {
        if (ql <= l && r <= qr) {
            tree[node].minVal += val;
            tree[node].lazy += val;
            return;
        }
        push(node);
        int mid = (l + r) / 2;
        if (ql <= mid) updateRange(2 * node, l, mid, ql, qr, val);
        if (qr > mid) updateRange(2 * node + 1, mid + 1, r, ql, qr, val);
        tree[node].minVal = Math.min(tree[2 * node].minVal, tree[2 * node + 1].minVal);
    }

    static void updatePoint(int node, int l, int r, int idx, long val) {
        if (l == r) {
            tree[node].minVal = val + tree[node].lazy;
            return;
        }
        push(node);
        int mid = (l + r) / 2;
        if (idx <= mid) updatePoint(2 * node, l, mid, idx, val);
        else updatePoint(2 * node + 1, mid + 1, r, idx, val);
        tree[node].minVal = Math.min(tree[2 * node].minVal, tree[2 * node + 1].minVal);
    }

    static class FastReader {
        BufferedReader br;
        StringTokenizer st;

        public FastReader() {
            br = new BufferedReader(new InputStreamReader(System.in));
        }

        String next() {
            while (st == null || !st.hasMoreElements()) {
                try {
                    st = new StringTokenizer(br.readLine());
                } catch (IOException e) {
                    return null;
                }
            }
            return st.nextToken();
        }

        int nextInt() {
            return Integer.parseInt(next());
        }
    }

    public static void main(String[] args) throws IOException {
        FastReader fr = new FastReader();
        String first = fr.next();
        if (first == null) return;
        int Q = Integer.parseInt(first);
        int[] X = new int[Q];
        int[] absX = new int[Q];
        for (int i = 0; i < Q; i++) {
            X[i] = fr.nextInt();
            absX[i] = Math.abs(X[i]);
        }

        int[] distinct = absX.clone();
        Arrays.sort(distinct);
        int m = 0;
        if (Q > 0) {
            m = 1;
            for (int i = 1; i < Q; i++) {
                if (distinct[i] != distinct[m - 1]) {
                    distinct[m++] = distinct[i];
                }
            }
        }
        
        if (m == 0) {
            PrintWriter out = new PrintWriter(System.out);
            for (int i = 0; i < Q; i++) out.print(0 + (i == Q - 1 ? "" : " "));
            out.println();
            out.flush();
            return;
        }

        nTree = 1;
        while (nTree < m) nTree *= 2;
        tree = new Node[2 * nTree];
        for (int i = 0; i < 2 * nTree; i++) tree[i] = new Node();

        int[] counts = new int[m];
        int currentN = 0;
        PrintWriter out = new PrintWriter(System.out);
        for (int i = 0; i < Q; i++) {
            int val = Math.abs(X[i]);
            int idx = Arrays.binarySearch(distinct, 0, m, val);
            if (X[i] > 0) {
                if (counts[idx] == 0) updatePoint(1, 0, nTree - 1, idx, (long) val);
                counts[idx]++;
                currentN++;
                updateRange(1, 0, nTree - 1, idx, m - 1, -1);
            } else {
                updateRange(1, 0, nTree - 1, idx, m - 1, 1);
                counts[idx]--;
                currentN--;
                if (counts[idx] == 0) updatePoint(1, 0, nTree - 1, idx, INF);
            }

            long tm = tree[1].minVal;
            long ans = currentN;
            if (tm < 0) ans += tm;
            out.print(ans + (i == Q - 1 ? "" : " "));
        }
        out.println();
        out.flush();
    }
}
