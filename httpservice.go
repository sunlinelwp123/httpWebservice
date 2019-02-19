package main
import (
	"fmt"
	"net/http"
	"strings"
	"log"
	"text/template"
	"os"
	"io/ioutil"
	"os/exec"
	"bytes"
	"strconv"
)

func httpHandler(w http.ResponseWriter, r *http.Request) {
	r.ParseForm()  //解析参数，默认是不会解析的
	fmt.Println(r.Form)  //这些信息是输出到服务器端的打印信息
	fmt.Println("path", r.URL.Path)
	fmt.Println("scheme", r.URL.Scheme)
	/*fmt.Println("header", r.Header)
	fmt.Println("r",r);*/
	body, _ := ioutil.ReadAll(r.Body)
        body_str := string(body)
        fmt.Println("body", body_str)
	for k, v := range r.Form {
		fmt.Println("key:", k)
		fmt.Println("val:", strings.Join(v, ""))
	}
	os.Setenv("REQUEST_METHOD", r.Method)
	if r.Method == "GET" {
		t,_ := template.ParseFiles("./webapp" + r.URL.Path)
		log.Println(t.Execute(w,nil))
	}else{
		log.Println(strconv.Itoa(len(body_str)))
		os.Setenv("CONTENT_LENGTH", strconv.Itoa(len(body_str)))
		cmd := exec.Command("./webapp" + r.URL.Path)
		cmd.Stdin = strings.NewReader(body_str)//重定向
		var out bytes.Buffer
		cmd.Stdout = &out
		err := cmd.Run()
		if err != nil {
			log.Fatal(err)
		}
		res := out.String()
		res_slic := res[strings.LastIndex(resp, "\r\n\r\n")+ 1:]
		fmt.Fprintf(w, res_slic)
	}
}

func userView(w http.ResponseWriter, r *http.Request) {
	fmt.Println("method:", r.Method) //获取请求的方法
	if r.Method == "GET" {
		t, _ := template.ParseFiles("./webapp/user.html")
		log.Println(t.Execute(w, nil))
	} else {
		//请求的是登录数据，那么执行登录的逻辑判断
		fmt.Println("server nil")
	}
}


func main() {
	http.HandleFunc("/", httpHandler) //设置访问的路由
	http.HandleFunc("/user", userView)
	err := http.ListenAndServe(":9090", nil) //设置监听的端口
	if err != nil {
		log.Fatal("ListenAndServe: ", err)
	}
}
