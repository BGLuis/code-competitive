import java.util.*;

public class Solution {
    static final int MOD = 998244353;

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        if (!sc.hasNextLong()) return;
        long N = sc.nextLong();
        int K = sc.nextInt();

        int[] dp = new int[K + 1];
        for (int q = 1; q <= K; q++) dp[q] = 1;

        int limit = (int) Math.min(N, (long) K);

        for (int i = 2; i <= limit; i++) {
            int[] newDpDiff = new int[K + 2];
            for (int qOld = 1; qOld <= K; qOld++) {
                if (dp[qOld] == 0) continue;

                int low = qOld - (qOld - 1) / i;
                int high = qOld + (K - qOld) / i;

                if (low <= K) {
                    newDpDiff[low] = (newDpDiff[low] + dp[qOld]) % MOD;
                    if (high + 1 <= K) {
                        newDpDiff[high + 1] = (newDpDiff[high + 1] - dp[qOld] + MOD) % MOD;
                    }
                }
            }
            int curr = 0;
            for (int q = 1; q <= K; q++) {
                curr = (curr + newDpDiff[q]) % MOD;
                dp[q] = curr;
            }
        }

        long total = 0;
        for (int q = 1; q <= K; q++) {
            total = (total + dp[q]) % MOD;
        }
        System.out.println(total);
    }
}
