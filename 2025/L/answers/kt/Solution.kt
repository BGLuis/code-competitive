import java.util.*

class Interval(val l: Double, val r: Double) : Comparable<Interval> {
    override fun compareTo(other: Interval): Int {
        if (Math.abs(l - other.l) > 1e-11) return l.compareTo(other.l)
        return r.compareTo(other.r)
    }
}

var bit = IntArray(0)
var mSize = 0

fun update(index: Int, value: Int) {
    var i = index
    while (i <= mSize) {
        bit[i] += value
        i += i and -i
    }
}

fun query(index: Int): Int {
    var i = index
    var res = 0
    while (i > 0) {
        res += bit[i]
        i -= i and -i
    }
    return res
}

fun findFirstGE(arr: DoubleArray, target: Double): Int {
    var low = 0
    var high = arr.size - 1
    var res = arr.size
    while (low <= high) {
        val mid = (low + high) / 2
        if (arr[mid] >= target - 1e-13) {
            res = mid
            high = mid - 1
        } else {
            low = mid + 1
        }
    }
    return res
}

fun main() {
    val sc = Scanner(System.`in`)
    if (!sc.hasNextInt()) return
    val n = sc.nextInt()
    val a = sc.nextDouble()
    val b = sc.nextDouble()

    val intervals = mutableListOf<Interval>()
    for (i in 0 until n) {
        val m = sc.nextDouble()
        val c = sc.nextDouble()
        val disc = m * m - 4.0 * a * (b - c)
        if (disc > 1e-11) {
            val sd = Math.sqrt(disc)
            val l = (m - sd) / (2.0 * a)
            val r = (m + sd) / (2.0 * a)
            intervals.add(Interval(l, r))
        }
    }

    if (intervals.isEmpty()) {
        println(0)
        return
    }

    Collections.sort(intervals)

    val allR = DoubleArray(intervals.size)
    for (i in intervals.indices) allR[i] = intervals[i].r
    Arrays.sort(allR)

    var m = 0
    if (allR.isNotEmpty()) {
        m = 1
        for (i in 1 until allR.size) {
            if (Math.abs(allR[i] - allR[m - 1]) > 1e-11) {
                allR[m++] = allR[i]
            }
        }
    }

    val finalAllR = allR.copyOf(m)
    mSize = m
    bit = IntArray(mSize + 1)

    var ans = 0L
    val eps = 1e-11
    for (inVal in intervals) {
        val idxL = findFirstGE(finalAllR, inVal.l + eps)
        val idxR = findFirstGE(finalAllR, inVal.r - eps)

        if (idxR > idxL) {
            ans += query(idxR) - query(idxL)
        }

        val rIdx = findFirstGE(finalAllR, inVal.r - 1e-13)
        update(rIdx + 1, 1)
    }
    println(ans)
}
