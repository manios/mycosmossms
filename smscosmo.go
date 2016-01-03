package main

import (
	"bytes"
	"crypto/tls"
	"fmt"
	"io/ioutil"
	"net/http"
	"net/url"
	"reflect"
	"regexp"
	"unicode/utf8"
)

type Stratos struct {
	ff int8
}

const MyCosmosUrl = "https://www.mycosmos.gr"

///
/// This is a main program!!! I am bobos and I like girls!
func main() {

	var ki int32
	var bi *int32

	ki = 12
	bi = &ki
	ki++

	*bi++
	fmt.Println("Dokimeio", *bi)

	var url string
	//	var roundCubeSessionId string

	url = "https://www.mycosmos.gr/"

	ki += 1

	transp := &http.Transport{
		TLSClientConfig: &tls.Config{InsecureSkipVerify: true},
	}

	client := &http.Client{Transport: transp}

	req, _ := http.NewRequest("GET", url, nil)
	// set custom header
	req.Header.Set("Bob", "bab")

	resp, err := client.Do(req)

	if err != nil {
		panic(err)
	}

	// defer connection close
	defer resp.Body.Close()

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

	var sessionCookie **http.Cookie

	for i, v := range cookies {
		fmt.Printf("pos:%d Name:%s -> %s\n", i, v.Name, v.Value)

		if v.Name == "roundcube_sessid" {
			sessionCookie = &v

			break
		}
	}

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

	fmt.Printf("%q\n", ret.FindAllStringSubmatch(string(body), 1))
	fmt.Printf("%s\n", ret.FindAllStringSubmatch(string(body), 1)[0][1])

	// http://blog.denevell.org/golang-regex-replace-split.html
	//	fmt.Println(testTheTest())

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

	cookies := resp.Cookies()
	var sessionCookie *http.Cookie

	for i, v := range cookies {
		fmt.Printf("pos:%d Name:%s -> %s\n", i, v.Name, v.Value)

		if v.Name == "roundcube_sessid" {
			sessionCookie = v

			break
		}
	}

	tokenPattern := regexp.MustCompile("request_token\":\"([^\"]+)\"")
	requestToken := tokenPattern.FindAllStringSubmatch(string(body), 1)[0][1]

	m := make(map[string]string)
	m["roundcube_sessid"] = sessionCookie.Value
	m["request_token"] = requestToken

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
	// http://stackoverflow.com/questions/12756782/go-http-post-and-use-cookies

	// connection params
	// skip SSL verification
	transp := &http.Transport{
		TLSClientConfig: &tls.Config{InsecureSkipVerify: true},
	}

	// create http client
	client := &http.Client{Transport: transp}

	req, _ := http.NewRequest("POST", MyCosmosUrl, bytes.NewBufferString(reqParams.Encode()))

	// execute POST request
	//resp, err := client.Do(req)
	_, err := client.Do(req)

	// if there is an error, panic
	// aaaaaa!
	if err != nil {
		panic(err)
	}

	// count number of characters (runes)
	//	utf8.RuneCountInString("Χρήστος世界"))
}
