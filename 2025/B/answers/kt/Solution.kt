import java.util.*

val MOD = 998244353

fun main() {
    val sc = Scanner(System.`in`)
    if (!sc.hasNextLong()) return
    val n = sc.nextLong()
    val k = sc.nextInt()

    var dp = IntArray(k + 1) { 1 }
    dp[0] = 0

    val limit = Math.min(n, k.toLong()).toInt()

    for (i in 2..limit) {
        val newDpDiff = IntArray(k + 2)
        for (qOld in 1..k) {
            if (dp[qOld] == 0) continue

            val low = qOld - (qOld - 1) / i
            val high = qOld + (k - qOld) / i

            if (low <= k) {
                newDpDiff[low] = (newDpDiff[low] + dp[qOld]) % MOD
                if (high + 1 <= k) {
                    newDpDiff[high + 1] = (newDpDiff[high + 1] - dp[qOld] + MOD) % MOD
                }
            }
        }
        var curr = 0
        for (q in 1..k) {
            curr = (curr + newDpDiff[q]) % MOD
            dp[q] = curr
        }
    }

    var total = 0L
    for (q in 1..k) {
        total = (total + dp[q]) % MOD
    }
    println(total)
}
