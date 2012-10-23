package main

import (
	"net/http"
	"log"
	// "fmt"
	"github.com/ajstarks/svgo"
	"math/rand"
	"math"
)

type Point2D struct {
	X float64
	Y float64
}

type BezierCurve struct {
	cps []Point2D
}

func add(p1 Point2D, p2 Point2D) Point2D {
	return Point2D{p1.X + p2.X, p1.Y+p2.Y}
}

func scale(val float64, p Point2D) Point2D {
	return Point2D{val * p.X, val * p.Y}
}

func factorial(n int) int {
	prod :=1
	for i:=1; i<=n; i++ {
		prod *= i
	}
	return prod
}
func comb(i int, n int) float64 {
	return float64(factorial(n)/(factorial(i)*factorial(n-i)))
}

func Bernstein(i int, n int, t float64) float64 {
	bni := comb(i,n)
	return bni * math.Pow(t, float64(i)) * math.Pow(1.0-t, float64(n-i))
}


func randomPoint2D() Point2D {
	return Point2D{rand.Float64(), rand.Float64()}
}

func bezP(t float64, pts []Point2D) Point2D {
	n := len(pts)
	rval := Point2D{0.0,0.0}
	for i := 0; i<n; i++ {
		rval = add(rval, scale(Bernstein(i,n-1,t), pts[i]))
	}
	return rval
}

func main() {
	http.Handle("/", http.HandlerFunc(show_beziers))
	err := http.ListenAndServe(":4040", nil)
	if err != nil {
		log.Println("ListenAndServe:", err)
	}
}

func mapPoint(p Point2D, oll Point2D, our Point2D, nll Point2D, nur Point2D) Point2D {
	oxdiff := (our.X - oll.X)
	nxdiff := (nur.X - nll.X)
	xp := p.X-oll.X
	xp = (nxdiff*xp)/oxdiff + nll.X
	
	oydiff := (oll.Y - our.Y)
	nydiff := (nll.Y - nur.Y)
	yp := p.Y-our.Y
	yp = (nydiff*yp)/oydiff+nur.Y
	
	return Point2D{xp, yp}
}

type PointMapper func(Point2D) Point2D

func ToSvg(bez *BezierCurve, s *svg.SVG, mp PointMapper) {
	
	// for idx := range(bez.cps) {
	// 	np := mp(bez.cps[idx])
	// 	// s.Circle(int(np.X), int(np.Y), 10,"fill:rgb(0,0,255);stroke:rgb(0,255,0)")
	// 	s.Text(int(np.X), int(np.Y), fmt.Sprintf("%v", idx), 
	// 		"text-anchor:middle;font-size:10pt;fill:black")
	// }
	tmin := 0.0
	tmax := 1.0
	steps := 120
	dt := (tmax-tmin)/float64(steps)
	t := tmin+dt

	pp := mp(bezP(0.0, bez.cps))
	s.Circle(int(pp.X), int(pp.Y), 1,"fill:rgb(0,255,0);stroke:rgb(255,0,0)")
	for i:=1;i<=steps;i++ {
		cp := bezP(t, bez.cps)
		mp := mp(cp)
		s.Circle(int(mp.X), int(mp.Y), 1,"fill:rgb(0,255,0);stroke:rgb(255,0,0)")
		s.Line(int(pp.X), int(pp.Y), int(mp.X), int(mp.Y), "stroke:rgb(0,0,0)")
		pp = mp
		t += dt
	}
	
}

func show_beziers(w http.ResponseWriter, req *http.Request) {
	if len(req.URL.Path) > 1 {
		return
	}

	w.Header().Set("Content-Type", "image/svg+xml")

	var randCurves [400]BezierCurve
	for i:=0; i<len(randCurves); i++ {
		randCurves[i] = BezierCurve{[]Point2D{ randomPoint2D(), randomPoint2D(),
				randomPoint2D(), randomPoint2D(),
			} }
	}
	s := svg.New(w)
	xmax := 1920
	ymax := 1080
	s.Start(xmax, ymax)
	s.Title("Bezier Curve")

	mmp := func(pt Point2D) Point2D { return mapPoint(pt, Point2D{-0.01,-0.01}, Point2D{1.01,1.01}, Point2D{0.0,0.0}, Point2D{float64(xmax),float64(ymax)}) }

	for j:=0; j<len(randCurves); j++ {
		ToSvg(&randCurves[j], s, mmp)
	}

	s.End()

}
