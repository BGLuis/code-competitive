import java.util.*;
import java.io.*;

public class Solution {
    public static void main(String[] args) throws IOException {
        Scanner sc = new Scanner(System.in);
        if (!sc.hasNextInt()) return;
        int n = sc.nextInt();
        long k = sc.nextLong();

        if (n == 1) {
            System.out.println(1);
            return;
        }

        long[] rs = new long[n];
        long[] cs = new long[n];
        long rMin = Long.MAX_VALUE, rMax = Long.MIN_VALUE;
        long cMin = Long.MAX_VALUE, cMax = Long.MIN_VALUE;

        for (int i = 0; i < n; i++) {
            rs[i] = sc.nextLong();
            cs[i] = sc.nextLong();
            if (rs[i] < rMin) rMin = rs[i];
            if (rs[i] > rMax) rMax = rs[i];
            if (cs[i] < cMin) cMin = cs[i];
            if (cs[i] > cMax) cMax = cs[i];
        }

        long h0 = rMax - rMin + 1;
        long w0 = cMax - cMin + 1;
        long ans = h0 * w0;

        for (int i = 0; i < n; i++) {
            long r = rs[i], c = cs[i];
            long au = rMax - r;
            long ad = r - rMin;
            long br = cMax - c;
            long bl = c - cMin;

            ans = Math.max(ans, (h0 + Math.max(0, k - au)) * (w0 + Math.max(0, k - br)));
            ans = Math.max(ans, (h0 + Math.max(0, k - au)) * (w0 + Math.max(0, k - bl)));
            ans = Math.max(ans, (h0 + Math.max(0, k - ad)) * (w0 + Math.max(0, k - br)));
            ans = Math.max(ans, (h0 + Math.max(0, k - ad)) * (w0 + Math.max(0, k - bl)));
        }

        long xOpt = (w0 + k - h0) / 2;
        long[] candidates = {0, k, xOpt, xOpt + 1};
        for (long x : candidates) {
            if (x >= 0 && x <= k) {
                ans = Math.max(ans, (h0 + x) * (w0 + k - x));
            }
        }

        System.out.println(ans);
    }
}
