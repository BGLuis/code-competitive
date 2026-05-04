import java.util.*

fun main() {
    val sc = Scanner(System.`in`)
    if (!sc.hasNextInt()) return
    val n = sc.nextInt()
    val k = sc.nextLong()

    if (n == 1) {
        println(1)
        return
    }

    val rs = LongArray(n)
    val cs = LongArray(n)
    var rMin = Long.MAX_VALUE
    var rMax = Long.MIN_VALUE
    var cMin = Long.MAX_VALUE
    var cMax = Long.MIN_VALUE

    for (i in 0 until n) {
        rs[i] = sc.nextLong()
        cs[i] = sc.nextLong()
        if (rs[i] < rMin) rMin = rs[i]
        if (rs[i] > rMax) rMax = rs[i]
        if (cs[i] < cMin) cMin = cs[i]
        if (cs[i] > cMax) cMax = cs[i]
    }

    val h0 = rMax - rMin + 1
    val w0 = cMax - cMin + 1
    var ans = h0 * w0

    for (i in 0 until n) {
        val r = rs[i]
        val c = cs[i]
        val au = rMax - r
        val ad = r - rMin
        val br = cMax - c
        val bl = c - cMin

        ans = Math.max(ans, (h0 + Math.max(0, k - au)) * (w0 + Math.max(0, k - br)))
        ans = Math.max(ans, (h0 + Math.max(0, k - au)) * (w0 + Math.max(0, k - bl)))
        ans = Math.max(ans, (h0 + Math.max(0, k - ad)) * (w0 + Math.max(0, k - br)))
        ans = Math.max(ans, (h0 + Math.max(0, k - ad)) * (w0 + Math.max(0, k - bl)))
    }

    val xOpt = (w0 + k - h0) / 2
    val candidates = longArrayOf(0, k, xOpt, xOpt + 1)
    for (x in candidates) {
        if (x in 0..k) {
            ans = Math.max(ans, (h0 + x) * (w0 + k - x))
        }
    }

    println(ans)
}
