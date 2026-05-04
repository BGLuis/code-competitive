import java.util.*;
import java.io.*;
import java.math.BigInteger;

public class Solution {
    static int N, M;
    static List<Integer>[] adj;
    static int[] sensorId;
    static List<BigInteger>[] failedMasks;
    static List<Integer> path = new ArrayList<>();

    static boolean dfs(int u, BigInteger forbiddenMask) {
        if (u == 2 * N) {
            path.add(u);
            return true;
        }

        for (BigInteger failed : failedMasks[u]) {
            if (forbiddenMask.and(failed).equals(failed)) return false;
        }

        for (int v : adj[u]) {
            int s = sensorId[v];
            if (forbiddenMask.testBit(s)) continue;

            if (dfs(v, forbiddenMask.setBit(s))) {
                path.add(u);
                return true;
            }
        }

        failedMasks[u].add(forbiddenMask);
        return false;
    }

    public static void main(String[] args) throws IOException {
        Scanner sc = new Scanner(System.`in`);
        if (!sc.hasNextInt()) return;
        N = sc.nextInt();
        M = sc.nextInt();

        adj = new ArrayList[2 * N + 1];
        failedMasks = new ArrayList[2 * N + 1];
        for (int i = 0; i <= 2 * N; i++) {
            adj[i] = new ArrayList<>();
            failedMasks[i] = new ArrayList<>();
        }

        sensorId = new int[2 * N + 1];
        for (int i = 1; i <= N; i++) {
            sensorId[i] = i;
            sensorId[i + N] = i;
        }

        for (int i = 0; i < M; i++) {
            int u = sc.nextInt();
            int v = sc.nextInt();
            adj[u].add(v);
        }

        BigInteger initialMask = BigInteger.ZERO.setBit(sensorId[1]);
        if (dfs(1, initialMask)) {
            Collections.reverse(path);
            System.out.println(path.size());
            for (int i = 0; i < path.size(); i++) {
                System.out.print(path.get(i) + (i == path.size() - 1 ? "" : " "));
            }
            System.out.println();
        } else {
            System.out.println("*");
        }
    }
}
