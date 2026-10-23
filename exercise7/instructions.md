# Exercise Session 7

Work through the exercises in order. Write answers in the `_Answer:_` blocks. Commit this
file and your `.cpp` files. Do **not** commit your Wi-Fi password (see Exercises 1 and 2).

Exercises 1-3 are for the session. Exercises 4 and 5 are homework.
See [`homework.md`](homework.md).

---

## Exercise 1: Cat facts over Wi-Fi

**Goal:** Join your hotspot and fetch a cat fact from the internet over HTTPS.

`cyw43_arch_...` functions drive the Wi-Fi chip. `cyw43_arch_enable_sta_mode()` lets the
Pico join a network: station mode (STA). In Exercise 2 it makes its own: access point (AP).

The starter fetches one fact each time you press the PicoBricks button.
You look at the request it sends and the response it gets.

This exercise uses the internet. The Pico W joins a network like a phone does. The
classroom network needs a login page, which the Pico can't fill in. So use the **hotspot
on your phone**:

- Set it to **2.4 GHz**. The Pico W can't use 5 GHz. On an iPhone, turn on **Maximize Compatibility**.
- Use **WPA2** security, not WPA3 only.

### Instructions

1. Create a project `catfact`. Copy all files from [`code/catfact/`](code/catfact/) into
   it.
2. In `CMakeLists.txt`, find `target_link_libraries(catfact ...)` with `pico_stdlib`.
   Remove any `pico_cyw43_arch_...` library the generator added. Then add these lines
   below it:

   ```cmake
   target_link_libraries(catfact
           pico_cyw43_arch_lwip_threadsafe_background
           pico_lwip_http
           pico_lwip_mbedtls
           pico_mbedtls
   )
   include(${CMAKE_CURRENT_LIST_DIR}/wifi.cmake)
   target_compile_definitions(catfact PRIVATE
           WIFI_SSID="${WIFI_SSID}"
           WIFI_PASSWORD="${WIFI_PASSWORD}"
   )
   ```

   Make a new file `wifi.cmake` next to `CMakeLists.txt`. Use your own hotspot name and
   password:

   ```cmake
   set(WIFI_SSID "your hotspot name")
   set(WIFI_PASSWORD "your password")
   ```

   The `.gitignore` in the repository lists `wifi.cmake`, so Git skips it. Run
   `git status`: `wifi.cmake` must not show up. If it does, add the line `wifi.cmake` to
   `.gitignore`.

   Add `example_http_client_util.c` to `add_executable(catfact ...)`. Build once. It takes
   a while: mbedTLS is big.
3. Read `main()`. Find where the Pico joins the hotspot, where it fills in `request`, and
   where it checks `result`.
4. Read `on_body()`. lwIP calls it once for each piece of the response. It adds the piece
   to `body`.
5. Turn on the hotspot. Open the Serial Monitor, build and run. Press the button. Each
   press prints a new fact.
6. Change `URL` to `"/nothing"`. Build, run and press the button. Read the output. Then
   put `"/fact"` back.
7. Remove the `extern "C" { }` around the `#include`. Build and read the error. Then put
   it back.

### Checklist

- [x] A new cat fact on each button press
- [x] Output for `/nothing` read (step 6)
- [x] Hotspot name and password only in `wifi.cmake`; `git status` does not list it
- [x] `extern "C"` error read (step 7)

**What does `request` hold before it is sent? Which function gets the response, and where
does the response end up?**

> _Answer: request contains the hostname, URL, callback function, TLS configuration, and a pointer to the response storage. The on_body() function receives the response in pieces and stores them in the body string._
>

**What is the `Body:` line? Which part does `extract_fact()` pick out, and how?**

> _Answer: The Body: line displays the full JSON response received from the server. extract_fact() searches for "fact":" and extracts the text between it and the next quotation mark, returning only the cat fact._
>

**What did step 6 print? What is `result` then? Where does the server say that something
went wrong?**

> _Answer: Changing the URL to /nothing should produce an HTTP 404 (Not Found) response. The result can still be 0 because the HTTP transfer completed successfully. The server indicates the error through the HTTP status code 404, shown as server_response 404 in the debug output._
>

**What is the difference between station mode (STA) and access point mode (AP)? Which
mode is your phone in while it runs the hotspot?**

> _Answer: Station mode (STA) allows a device to connect to an existing Wi-Fi network, while access point mode (AP) allows a device to create a network for others to connect to. The Pico uses STA mode, while the phone acts as an AP when running the hotspot._
>

**What was the error in step 7? Is it from the compiler or the linker? Why?**

> _Answer: Removing extern "C" causes a linker error, usually an undefined reference to http_client_request_sync. This happens because C++ uses name mangling, while the function is implemented in C. The linker cannot match the function names without extern "C"._
>

**The TLS configuration gets no certificate (`nullptr, 0`). The data is encrypted. Can
you be sure you talk to the real catfact.ninja?**

> _Answer: No. Although TLS encrypts the connection, without certificate verification the Pico cannot confirm the server's identity. This means an attacker could impersonate the server and intercept the communication._
>

**Why are the hotspot name and password in `wifi.cmake`, not in `catfact.cpp` or
`CMakeLists.txt`? What happens if you delete `wifi.cmake` and build?**

> _Answer: The credentials are stored separately in wifi.cmake to keep sensitive information out of the source code and Git repository. This also makes changing networks easier. If wifi.cmake is deleted, CMake cannot include the file, so project configuration fails and the build cannot complete normally._
>

**Attached file(s):**

> _Filename: Exercise1 folder_
>

*Read more (optional): Real-Time C++, Sect. 6.6 ("Understand Name Mangling and
De-mangling"), pp. 121-123: why C++ names don't match C names. Beginning C++17,
Chapter 7 ("Searching Strings", "Accessing Substrings"): `find()` and `substr()` used in
`extract_fact()`.*

---

## Exercise 2: A web server on the Pico

**Goal:** Show the Pico's status in the browser and switch the red LED from there.

Now the Pico is the server. The starter has a class for it, `HTTPServer`, in
`Connection.h` and `Connection.cpp`. It uses lwIP's web server, **httpd**. The page is
`content/index.shtml`. CMake builds it into flash.

- **SSI**: the server replaces a tag like `<!--#uptime-->` with text from your function.
  Tags are at most 8 characters.
- **CGI**: a path like `/led/on` calls your function. It returns the page to send.

The starter writes each handler as a lambda: `[]() -> std::string { return "ON"; }`.

The Pico makes its own network here (AP), so no phone is needed. Your PC joins it and has
no internet meanwhile. Pick a unique name: if a network nearby already has it, the Pico
stops.

### Instructions

1. Create a project `webserver`. Copy all files and the `content` folder from
   [`code/webserver/`](code/webserver/) into it. `webserver.cpp` replaces the generated one.
   Create a new `wifi.cmake` next to `CMakeLists.txt`, with a name for the Pico's network:

   ```cmake
   set(AP_SSID "a unique name")
   set(AP_PASSWORD "8 characters or more")
   ```
2. In `CMakeLists.txt`, add `Connection.cpp` to `add_executable(webserver ...)`. Remove any
   `pico_cyw43_arch_...` library, like in Exercise 1. Then add below `target_link_libraries`:

   ```cmake
   target_link_libraries(webserver
           pico_cyw43_arch_lwip_threadsafe_background
           pico_lwip_http
           pico_lwip_mdns
           pico_httpd_content
           hardware_adc
   )
   pico_add_library(pico_httpd_content NOFLAG)
   pico_set_lwip_httpd_content(pico_httpd_content INTERFACE
           ${CMAKE_CURRENT_LIST_DIR}/content/404.html
           ${CMAKE_CURRENT_LIST_DIR}/content/index.shtml
   )
   include(${CMAKE_CURRENT_LIST_DIR}/wifi.cmake)
   target_compile_definitions(webserver PRIVATE
           AP_SSID="${AP_SSID}"
           AP_PASSWORD="${AP_PASSWORD}"
   )
   target_sources(webserver PRIVATE
           ${PICO_SDK_PATH}/lib/btstack/3rd-party/lwip/dhcp-server/dhserver.c
   )
   target_include_directories(webserver PRIVATE
           ${PICO_SDK_PATH}/lib/btstack/3rd-party/lwip/dhcp-server
   )
   ```

   `dhserver.c` is a small DHCP server from the SDK. It gives your PC its address.
3. Open the Serial Monitor **before** you run. Build and run. Join the Pico's network with
   your PC. Open the address from the `Open in your browser:` box. Temperature shows
   `***UNKNOWN TAG temp***`.
4. Fill in TODO 1. Copy the `"button"` SSI and change it. Use `read_temperature()`.
   `std::to_string()` turns a number into text.
5. Fill in TODO 2. Copy the `"/led/on"` CGI and change it. `gpio_put()` switches the LED.
6. Build and run. Hold the button and press F5 to reload the page. Then click **LED on**
   and **LED off**.

### Checklist

- [x] The page opens on your PC
- [x] TODO 1 and 2 filled in; the page shows the temperature
- [x] **LED on** and **LED off** switch the red LED

**Find `<!--#uptime-->` in `index.shtml`. Which function makes its text? When does it
run? Why does the number change only when you reload?**

> _Answer: The HTTPServer::AddSSIHandler() function registers the handler that generates the uptime text. It runs whenever the server processes a request for index.shtml. The number only changes when we reload because the page is static after loading and doesn't automatically request updated values._
>

**Set `AP_SSID` to the name of a network near you, like your phone's hotspot. Build and
run. What does the Pico print? Put your own name back.**

> _Answer: If we set AP_SSID to the name of a nearby Wi-Fi network, the Pico detects that the name is already being used and prints a warning about the duplicate SSID. We should change it back to our own unique name to avoid conflicts._
>

**Click LED on, then LED off. Use the back button once and press F5. What does the
browser ask for, and what happens to the LED? A REST API would not change the LED on a
GET. Why is that a good rule?**

> _Answer: When we click LED on or off, the browser sends a GET request to /led/on or /led/off. Pressing Back and then F5 can repeat the previous request, changing the LED again. In a REST API, GET requests should only retrieve information, not modify anything. This prevents accidental changes when users refresh pages, revisit links, or when browsers automatically fetch URLs._
>

**Attached file(s):**

> _Filename: Exercise 2 folder_
>

*Read more (optional): Beginning C++17, Chapter 18 ("Lambda Expressions", "The
std::function<> Template"): the handlers in `HTTPServer`. Real-Time C++, Sect. 6.18 ("Use
Lambda Expressions"), pp. 139-140.*

---

## Exercise 3: A Fraction class with operators

**Goal:** Give a `Fraction` class the operators `+=`, `+`, `*` and `==`.

`a + b` calls `a.operator+(b)`. The starter has `Fraction& operator+=(const Fraction& other)`:
it changes `*this` and returns it. Below the class, the starter has a non-member
`operator-` as an example. You write:

- `Fraction operator+(const Fraction& other) const`: a member. Returns a new `Fraction`.
- `Fraction operator*(int factor, const Fraction& fraction)`: a non-member, for an `int`
  on the left.

The starter keeps every `Fraction` reduced with `std::gcd()`: `2/4` becomes `1/2`. A sum
`a/b + c/d` is `(a*d + c*b) / (b*d)`.

### Instructions

1. Create a project `fraction`. Copy in [`code/fraction.cpp`](code/fraction.cpp).
2. Read the class. Find the constructor, `reduce()` and `print()`.
3. Read `operator+=`. It changes `numerator_` and `denominator_`, then calls `reduce()`.
4. Fill in TODO 1. `a + b` must not change `a`, so work on a copy:
   `Fraction result {*this};`. Add `other` to `result` with `+=`. Return `result`.
5. Fill in TODO 2. `operator*` builds a new `Fraction`.
6. Fill in TODO 3. `operator==` compares both members.
7. Read the non-member `operator-` below the class. Fill in TODO 4 the same way: a
   non-member `operator*`. Use the member one.
8. Fill in TODO 5. Open the Serial Monitor, build and run. Fill in the table.
9. Comment out your non-member `operator*`. Build and read the error. Put it back.
10. Add the line `show("half + 1 = ", half + 1);`. Build and read the error. Remove it.

| Line | Your output |
|---|---|
| `half + third` |5/6 |
| `half * 3` | 3/2|
| `3 * half` | 3/2|
| `sum` | 1/1|
| `sum == 1/1` | yes|

### Checklist

- [x] TODO 1-5 filled in; the table filled in
- [x] `operator+` uses `operator+=`
- [x] Errors read (steps 9 and 10)

**Why does `operator+=` return `Fraction&`, but `operator+` returns `Fraction`?**

> _Answer: operator+= modifies the existing fraction, so it returns a reference (Fraction&) to the same object. operator+ creates a new fraction without changing the original ones, so it returns a new Fraction by value._
>

**What was the error in step 9? Why can't `3 * half` use a member function?**

> _Answer: The compiler reported that there was no matching operator* for int and Fraction. A member operator belongs to the object on the left side, but 3 is an integer, not a Fraction. That's why we need a non-member operator*._
>

**What was the error in step 10? Which operator would make `half + 1` work?**

> _Answer: The compiler reported no matching operator+ because our operator expects another Fraction, not an integer. Adding an overloaded operator+(int) to the Fraction class would make half + 1 work._
>

**`sum` is `1/3 + 1/3 + 1/3`. What would `sum == Fraction {1, 1}` give without
`reduce()`? Why?**

> _Answer: It would return false because without reduce(), the sum would be stored as 27/27 instead of 1/1. Our operator== compares the numerator and denominator directly, so it would consider them different even though they represent the same value._
>

**Attached file(s):**

> _Filename: fraction.cpp_
>

*Read more (optional): Beginning C++17, Chapter 12 ("Implementing an Overloaded
Operator", "Nonmember Operator Functions", "Implementing One Operator in Terms of
Another", "Member vs. Nonmember Functions"). Real-Time C++, Sect. 13.2 ("A Scalable
Fixed-Point Template Class"), pp. 270-273: the operators a number class should have.*

Exercises 4 and 5 continue in [`homework.md`](homework.md).
