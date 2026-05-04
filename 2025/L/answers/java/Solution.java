import java.util.*;
import java.io.*;

public class Solution {
    static class Interval implements Comparable<Interval> {
        double l, r;
        public Interval(double l, double r) { this.l = l; this.r = r; }
        @Override
        public int compareTo(Interval other) {
            if (Math.abs(this.l - other.l) > 1e-11) return Double.compare(this.l, other.l);
            return Double.compare(this.r, other.r);
        }
    }

    static int[] bit;
    static int mSize;

    static void update(int i, int val) {
        for (; i <= mSize; i += i & -i) bit[i] += val;
    }

    static int query(int i) {
        int res = 0;
        for (; i > 0; i -= i & -i) res += bit[i];
        return res;
    }

    static int findFirstGE(double[] arr, double target) {
        int low = 0, high = arr.length - 1;
        int res = arr.length;
        while (low <= high) {
            int mid = (low + high) / 2;
            if (arr[mid] >= target - 1e-13) {
                res = mid;
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }
        return res;
    }

    public static void main(String[] args) throws IOException {
        Scanner sc = new Scanner(System.`in`);
        if (!sc.hasNextInt()) return;
        int n = sc.nextInt();
        double a = sc.nextDouble();
        double b = sc.nextDouble();

        List<Interval> intervals = new ArrayList<>();
        for (int i = 0; i < n; i++) {
            double m = sc.nextDouble();
            double c = sc.nextDouble();
            double disc = m * m - 4.0 * a * (b - c);
            if (disc > 1e-11) {
                double sd = Math.sqrt(disc);
                double l = (m - sd) / (2.0 * a);
                double r = (m + sd) / (2.0 * a);
                intervals.add(new Interval(l, r));
            }
        }

        if (intervals.isEmpty()) {
            System.out.println(0);
            return;
        }

        Collections.sort(intervals);

        double[] allR = new double[intervals.size()];
        for (int i = 0; i < intervals.size(); i++) allR[i] = intervals.get(i).r;
        Arrays.sort(allR);
        
        int m = 0;
        if (allR.length > 0) {
            m = 1;
            for (int i = 1; i < allR.length; i++) {
                if (Math.abs(allR[i] - allR[m-1]) > 1e-11) {
                    allR[m++] = allR[i];
                }
            }
        }
        
        double[] finalAllR = Arrays.copyOf(allR, m);
        mSize = m;
        bit = new int[mSize + 1];

        long ans = 0;
        double eps = 1e-11;
        for (Interval in : intervals) {
            int idxL = findFirstGE(finalAllR, in.l + eps);
            int idxR = findFirstGE(finalAllR, in.r - eps); 
            // idxL is first index where R_j > L_i + eps
            // idxR is first index where R_j >= R_i - eps
            // We want indices j in [idxL, idxR-1]
            
            if (idxR > idxL) {
                ans += query(idxR) - query(idxL);
            }
            
            int rIdx = findFirstGE(finalAllR, in.r - 1e-13);
            update(rIdx + 1, 1);
        }
        System.out.println(ans);
    }
}
