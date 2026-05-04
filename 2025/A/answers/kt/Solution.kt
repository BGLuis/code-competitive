import java.util.*

fun main() {
    val sc = Scanner(System.`in`)
    if (!sc.hasNextInt()) return
    val n = sc.nextInt()

    val p = sc.nextInt()
    val l = IntArray(p)
    for (i in 0 until p) l[i] = sc.nextInt()

    val q = sc.nextInt()
    val r = IntArray(q)
    for (i in 0 until q) r[i] = sc.nextInt()

    if (n % 2 == 0) {
        if (n > 2) {
            println("N")
        } else {
            // N = 2, S = 2
            if (p + q > 2) println("N")
            else if (p >= 2 && l[0] == l[1]) println("N")
            else if (q >= 2 && r[0] == r[1]) println("N")
            else if (p >= 1 && q >= 1 && l[0] == r[0]) println("N")
            else println("Y")
        }
        return
    }

    val s = n.toLong() * (n - 1) / 2 + 1
    if (p + q > s) {
        println("N")
        return
    }

    for (i in 0 until p - 1) if (l[i] == l[i + 1]) { println("N"); return }
    for (i in 0 until q - 1) if (r[i] == r[i + 1]) { println("N"); return }

    if (p > 0 && q > 0 && l[0] != r[q - 1]) {
        println("N")
        return
    }

    val usedEdgesMat = Array(n + 1) { BooleanArray(n + 1) }
    var usedCount = 0
    for (i in 0 until p - 1) {
        var u = l[i]
        var v = l[i + 1]
        if (u > v) { val tmp = u; u = v; v = tmp }
        if (usedEdgesMat[u][v]) { println("N"); return }
        usedEdgesMat[u][v] = true
        usedCount++
    }
    for (i in 0 until q - 1) {
        var u = r[i]
        var v = r[i + 1]
        if (u > v) { val tmp = u; u = v; v = tmp }
        if (usedEdgesMat[u][v]) { println("N"); return }
        usedEdgesMat[u][v] = true
        usedCount++
    }

    val ePrime = (s - 1) - usedCount
    val uTarget: Int
    val vTarget: Int
    if (p > 0 && q > 0) { uTarget = l[p - 1]; vTarget = r[0] }
    else if (p > 0) { uTarget = l[p - 1]; vTarget = l[0] }
    else if (q > 0) { uTarget = r[q - 1]; vTarget = r[0] }
    else { uTarget = 1; vTarget = 1 }

    if (ePrime == 0L) {
        println(if (uTarget == vTarget) "Y" else "N")
        return
    }

    val parent = IntArray(n + 1) { it }
    fun find(i: Int): Int {
        if (parent[i] == i) return i
        parent[i] = find(parent[i])
        return parent[i]
    }
    fun unite(i: Int, j: Int) {
        val rootI = find(i)
        val rootJ = find(j)
        if (rootI != rootJ) parent[rootI] = rootJ
    }

    val hasEdge = BooleanArray(n + 1)
    for (i in 1..n) {
        for (j in i + 1..n) {
            if (!usedEdgesMat[i][j]) {
                unite(i, j)
                hasEdge[i] = true
                hasEdge[j] = true
            }
        }
    }

    var edgeComp = -1
    for (i in 1..n) {
        if (hasEdge[i]) {
            val c = find(i)
            if (edgeComp == -1) edgeComp = c
            else if (edgeComp != c) { println("N"); return }
        }
    }

    if (edgeComp != -1) {
        if (find(uTarget) != edgeComp || find(vTarget) != edgeComp) {
            println("N")
            return
        }
    }

    println("Y")
}
