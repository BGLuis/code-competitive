import java.util.*;
import java.io.*;
import java.math.BigInteger;

public class Solution {
    static class Cleaner {
        int h, l, u;
        long m;
        public Cleaner(int h, int l, int u, long m) {
            this.h = h; this.l = l; this.u = u; this.m = m;
        }
    }

    static class Rational implements Comparable<Rational> {
        int num, den;
        public Rational(int num, int den) {
            this.num = num; this.den = den;
        }
        @Override
        public int compareTo(Rational other) {
            long p1 = (long) num * other.den;
            long p2 = (long) other.num * den;
            return Long.compare(p1, p2);
        }
        @Override
        public boolean equals(Object obj) {
            if (!(obj instanceof Rational)) return false;
            Rational other = (Rational) obj;
            return (long) num * other.den == (long) other.num * den;
        }
    }

    static class Event implements Comparable<Event> {
        Rational coord;
        int type; // 0 start, 1 end
        int id;
        public Event(Rational coord, int type, int id) {
            this.coord = coord; this.type = type; this.id = id;
        }
        @Override
        public int compareTo(Event other) {
            int cmp = this.coord.compareTo(other.coord);
            if (cmp != 0) return cmp;
            return Integer.compare(this.type, other.type);
        }
    }

    static long[] bitCnt;
    static long[] bitSum;
    static int nh;
    static int[] distinctH;

    static void update(int idx, long valM, long valMH) {
        for (; idx <= nh; idx += idx & -idx) {
            bitCnt[idx] += valM;
            bitSum[idx] += valMH;
        }
    }

    static long getMinSumH(long s) {
        int idx = 0;
        long currentCnt = 0;
        long currentSum = 0;
        int maxBit = Integer.highestOneBit(nh);
        for (int i = maxBit; i > 0; i >>= 1) {
            int nextIdx = idx + i;
            if (nextIdx <= nh && currentCnt + bitCnt[nextIdx] < s) {
                idx = nextIdx;
                currentCnt += bitCnt[idx];
                currentSum += bitSum[idx];
            }
        }
        long remaining = s - currentCnt;
        currentSum += remaining * distinctH[idx];
        return currentSum;
    }

    public static void main(String[] args) throws IOException {
        Scanner sc = new Scanner(System.in);
        if (!sc.hasNextInt()) return;
        int n = sc.nextInt();
        long s = sc.nextLong();
        long k = sc.nextLong();

        List<Cleaner> cleaners = new ArrayList<>();
        List<Event> events = new ArrayList<>();
        Set<Integer> hSet = new TreeSet<>();

        for (int i = 0; i < n; i++) {
            int h = sc.nextInt();
            int l = sc.nextInt();
            int u = sc.nextInt();
            long m = k / h;
            if (m >= 1) {
                int id = cleaners.size();
                cleaners.add(new Cleaner(h, l, u, m));
                events.add(new Event(new Rational(l, h), 0, id));
                events.add(new Event(new Rational(u, h), 1, id));
                hSet.add(h);
            }
        }

        if (cleaners.isEmpty()) {
            System.out.println("*");
            return;
        }

        distinctH = new int[hSet.size()];
        int hPtr = 0;
        for (int h : hSet) distinctH[hPtr++] = h;
        nh = distinctH.length;

        bitCnt = new long[nh + 1];
        bitSum = new long[nh + 1];

        Collections.sort(events);

        long bestNum = -1, bestDen = 1;

        int i = 0;
        while (i < events.size()) {
            Rational currentCoord = events.get(i).coord;
            int j = i;
            while (j < events.size() && events.get(j).coord.equals(currentCoord)) {
                if (events.get(j).type == 0) {
                    Cleaner c = cleaners.get(events.get(j).id);
                    int hIdx = Arrays.binarySearch(distinctH, c.h) + 1;
                    update(hIdx, c.m, c.m * c.h);
                }
                j++;
            }

            long totalCnt = 0;
            for (int idx = nh; idx > 0; idx -= idx & -idx) totalCnt += bitCnt[idx];

            if (totalCnt >= s) {
                long sumH = getMinSumH(s);
                long curNum = (long) currentCoord.num * sumH;
                long curDen = currentCoord.den;
                if (bestNum == -1 || compareFractions(curNum, curDen, bestNum, bestDen) < 0) {
                    bestNum = curNum;
                    bestDen = curDen;
                }
            }

            j = i;
            while (j < events.size() && events.get(j).coord.equals(currentCoord)) {
                if (events.get(j).type == 1) {
                    Cleaner c = cleaners.get(events.get(j).id);
                    int hIdx = Arrays.binarySearch(distinctH, c.h) + 1;
                    update(hIdx, -c.m, -c.m * c.h);
                }
                j++;
            }
            i = j;
        }

        if (bestNum == -1) {
            System.out.println("*");
        } else {
            long common = gcd(bestNum, bestDen);
            System.out.println((bestNum / common) + " " + (bestDen / common));
        }
    }

    static int compareFractions(long n1, long d1, long n2, long d2) {
        BigInteger p1 = BigInteger.valueOf(n1).multiply(BigInteger.valueOf(d2));
        BigInteger p2 = BigInteger.valueOf(n2).multiply(BigInteger.valueOf(d1));
        return p1.compareTo(p2);
    }

    static long gcd(long a, long b) {
        return b == 0 ? a : gcd(b, a % b);
    }
}
