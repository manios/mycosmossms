package main

import (
	"bytes"
	"crypto/tls"
	"fmt"
	"io/ioutil"
	"net/http"
	"net/http/httputil"
	"net/url"
	"reflect"
	"regexp"
	"strconv"
	"strings"
	"unicode/utf8"
)

type Stratos struct {
	ff int8
}

type Credentials struct {
	Username string
	password string
}

type MyCosmosSession struct {
	RequestToken string
	SessionId    string
	SessionAuth  string
}

const MyCosmosUrl = "https://www.mycosmos.gr/"

var mycookies []*http.Cookie

///
/// This is a main program!!! I am bobos and I like girls!
func main() {

	var requestToken string
	var roundCubeSessionId string

	// get sessionIds
	respMap := getSessionId(MyCosmosUrl)

	requestToken = respMap["request_token"]
	roundCubeSessionId = respMap["roundcube_sessid"]

	fmt.Printf("getSessionId roundcube sessId:%s\nrequest_token:%s\n", roundCubeSessionId, requestToken)

	login("", "", requestToken, roundCubeSessionId)

	// JSON post
	// http://stackoverflow.com/questions/24455147/go-lang-how-send-json-string-in-post-request

	// url encoding in POST request
	// http://stackoverflow.com/questions/19253469/make-a-url-encoded-post-request-using-http-newrequest

	/**
	 * example from:
	 * https://github.com/StefanSchroeder/Golang-Regex-Tutorial/blob/master/01-chapter2.markdown
	 */

	// Golang Https
	// http://stackoverflow.com/questions/12122159/golang-how-to-do-a-https-request-with-bad-certificate
}

func getSessionId(myurl string) map[string]string {

	// connection params
	// skip SSL verification
	transp := &http.Transport{
		TLSClientConfig: &tls.Config{InsecureSkipVerify: true},
	}

	// create http client
	client := &http.Client{Transport: transp}

	req, _ := http.NewRequest("GET", myurl, nil)

	// execute GET request
	resp, err := client.Do(req)

	// if there is an error, panic
	// aaaaaa!
	if err != nil {
		panic(err)
	}

	// read http response body
	body, _ := ioutil.ReadAll(resp.Body)

	// defer connection close
	defer resp.Body.Close()

	//cookies := resp.Cookies()

	mycookies = resp.Cookies()
	var sessionCookie *http.Cookie

	//	for i, v := range cookies {
	for i, v := range mycookies {
		fmt.Printf("pos:%d Name:%s -> %s\n", i, v.Name, v.Value)

		if v.Name == "roundcube_sessid" {
			sessionCookie = v
			//					break
		}
	}

	tokenPattern := regexp.MustCompile("request_token\":\"([^\"]+)\"")
	requestToken := tokenPattern.FindAllStringSubmatch(string(body), 1)[0][1]

	m := make(map[string]string)
	m["roundcube_sessid"] = sessionCookie.Value
	m["request_token"] = requestToken

	//	fmt.Printf("getSessionId roundcube sessId:%s\nrequest_token:%s", m["roundcube_sessid"], m["request_token"])

	return m
}

func login(username string, password string, requestToken string, roundCubeSessionId string) {

	reqParams := url.Values{}

	reqParams.Set("_token", requestToken)
	reqParams.Set("_task", "login")
	reqParams.Set("_action", "login")
	reqParams.Set("_timezone", "Europe/Helsinki")
	reqParams.Set("_url", "")
	reqParams.Set("_user", username)
	reqParams.Set("_pass", password)

	// TODO add cookies
	//	myCookie := http.Cookie{
	//		Name:     "roundcube_sessid",
	//		Value:    roundCubeSessionId,
	//		Path:     "/",
	//		MaxAge:   0,
	//		Secure:   true,
	//		HttpOnly: true,
	//		Raw:      "roundcube_sessid" + roundCubeSessionId,
	//		Unparsed: []string{"roundcube_sessid" + roundCubeSessionId}}

	// http://stackoverflow.com/questions/12756782/go-http-post-and-use-cookies
	// https://groups.google.com/forum/#!topic/golang-nuts/x3JDvuWVO9A
	// http://stackoverflow.com/questions/12130582/setting-cookies-in-golang-net-http

	// connection params
	// skip SSL verification
	transp := &http.Transport{
		TLSClientConfig: &tls.Config{InsecureSkipVerify: true},
	}

	// create http client
	//	client := &http.Client{Transport: transp}

	req, _ := http.NewRequest("POST", MyCosmosUrl, bytes.NewBufferString(reqParams.Encode()))
	req.Header.Set("Content-Type", "application/x-www-form-urlencoded")
	req.Header.Set("User-Agent", "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:44.0) Gecko/20100101 Firefox/44.0")
	req.Header.Set("Content-Length", strconv.Itoa(len(reqParams.Encode())))
	req.Header.Set("Accept-Encoding", "gzip, deflate, br")
	// comment it for a while
	//req.AddCookie(&myCookie)

	for i, v := range mycookies {
		if v.Name == "roundcube_sessid" {

			fmt.Printf("Add cookie pos:%d Name:%s -> %s\n", i, v.Name, v.Value)
			req.AddCookie(v)
		}
	}

	debug(httputil.DumpRequestOut(req, true))

	// http://stackoverflow.com/questions/23297520/how-can-i-make-the-go-http-client-not-follow-redirects-automatically
	// http://speakmy.name/2014/07/29/http-request-debugging-in-go/

	// execute POST request
	//	resp, err := client.Do(req)
	resp, err := transp.RoundTrip(req)
	//_, err := client.Do(req)

	// if there is an error, panic
	// aaaaaa!
	if err != nil {
		panic(err)
	}

	fmt.Println("====== DEBUG LOGIN INFORMATION =======")
	debugLoginHttpResponse(resp)

	// we do not need to read http response body
	// as it contains no information

	//cookies := resp.Cookies()
	mycookies = resp.Cookies()
	var sessionIdCookie *http.Cookie
	var sessionAuthCookie *http.Cookie

	for i, v := range mycookies {
		fmt.Printf("pos:%d Name:%s -> %s\n", i, v.Name, v.Value)

		if v.Name == "roundcube_sessid" {
			sessionIdCookie = v
		}

		if v.Name == "roundcube_sessauth" {
			sessionAuthCookie = v
		}

	}

}

// Returns true if the given string has a length >= limit, or false otherwise.
// If string is nil or empty it also returns false
func isStringLeLimit(str string, limit int) bool {

	if strings.TrimSpace(str) == "" {
		return false
	}

	// count number of characters (runes)
	strlen := utf8.RuneCountInString(str)

	if (strlen <= 0) || (strlen >= limit) {
		return false
	}

	return true
}

func debugLoginHttpResponse(resp *http.Response) {

	body, _ := ioutil.ReadAll(resp.Body)
	//	fmt.Println("response Body:", string(body))

	fmt.Println("response Status:", resp.Status)

	fmt.Println(resp.Status == "200 OK")
	fmt.Println("response Headers:", resp.Header)

	cookies := resp.Cookies()
	fmt.Println("Response Cookies:", cookies)

	fmt.Println("Response Cookies:", reflect.TypeOf(cookies))

	var roundCubeSessionId = resp.Header["Set-Cookie"]
	fmt.Println("roundCubeSessionId: ", roundCubeSessionId)

	var newLocation = resp.Header["Location"]
	fmt.Println("newLocation: ", newLocation)

	var sessionCookie **http.Cookie

	for i, v := range cookies {
		fmt.Printf("pos:%d Name:%s -> %s\n", i, v.Name, v.Value)

		if v.Name == "roundcube_sessid" {
			sessionCookie = &v

			//			break
		}
	}

	//	var beba string
	//	beba = "Haos"
	//	fmt.Println("beba: ", beba)

	fmt.Printf("found cookie: %t\n", (*sessionCookie != nil))
	//	fmt.Printf("Name:%s -> %s\n", *sessionCookie.Name, *sessionCookie.Value)

	// JSON post
	// http://stackoverflow.com/questions/24455147/go-lang-how-send-json-string-in-post-request

	// url encoding in POST request
	// http://stackoverflow.com/questions/19253469/make-a-url-encoded-post-request-using-http-newrequest

	/**
	 * example from:
	 * https://github.com/StefanSchroeder/Golang-Regex-Tutorial/blob/master/01-chapter2.markdown
	 */

	//	var patterno = "`(?m)^.*request_token.:.([^\"]+)\".*$`"

	//	re := regexp.MustCompile(patterno)
	ret := regexp.MustCompile("request_token\":\"([^\"]+)\"")

	fmt.Printf("Full body:\n%s\n", string(body))
	fmt.Printf("request_token:%q\n", ret.FindAllStringSubmatch(string(body), 1))
	//fmt.Printf("%s\n", ret.FindAllStringSubmatch(string(body), 1)[0][1])

	// http://blog.denevell.org/golang-regex-replace-split.html
	//	fmt.Println(testTheTest())

	// Golang Https
	// http://stackoverflow.com/questions/12122159/golang-how-to-do-a-https-request-with-bad-certificate
}

func debug(data []byte, err error) {

	if err == nil {
		fmt.Printf("%s\n\n", data)
	} else {
		fmt.Printf("%s\n\n", err)
	}

}

// idle_timeout_warning":15,"idle_timeout_logout":5,"request_token":"765812d0cd2327d9044f2e7939356c74"});
// rcmail.display_message("Operation completed, total sent messages: 1","notice",0);
