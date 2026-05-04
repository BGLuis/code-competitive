import java.util.*;
import java.io.*;

public class Solution {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        if (!sc.hasNext()) return;
        String s = sc.next();
        int n = s.length();
        
        Map<Long, Long> factors = new TreeMap<>();
        if (n <= 15) {
            long val = Long.parseLong(s);
            factorize(val, factors);
        } else {
            // Take first 14 digits to keep sqrt(P) small enough for trial division
            long p = Long.parseLong(s.substring(0, 14));
            long l = n - 14;
            factorize(p, factors);
            factors.put(2L, factors.getOrDefault(2L, 0L) + l);
            factors.put(5L, factors.getOrDefault(5L, 0L) + l);
        }
        
        List<Map.Entry<Long, Long>> res = new ArrayList<>();
        for (Map.Entry<Long, Long> entry : factors.entrySet()) {
            if (entry.getValue() > 0) res.add(entry);
        }
        
        System.out.println(res.size());
        for (Map.Entry<Long, Long> entry : res) {
            System.out.println(entry.getKey() + " " + entry.getValue());
        }
    }

    static void factorize(long n, Map<Long, Long> factors) {
        for (long i = 2; i * i <= n; i++) {
            while (n % i == 0) {
                factors.put(i, factors.getOrDefault(i, 0L) + 1);
                n /= i;
            }
        }
        if (n > 1) factors.put(n, factors.getOrDefault(n, 0L) + 1);
    }
}
