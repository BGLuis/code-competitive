import java.util.*;

public class Solution {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        if (!sc.hasNext()) return;
        String s = sc.next();
        
        long score = 0;
        int n = s.length();
        for (int i = 0; i < n; i++) {
            if (s.startsWith("ha", i)) score += 1;
            if (s.startsWith("boooo", i)) score -= 1;
            if (s.startsWith("bravo", i)) score += 3;
        }
        System.out.println(score);
    }
}
