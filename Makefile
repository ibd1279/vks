all:
	./header-generator/header-generator > /tmp/test.go && gofmt /tmp/test.go > /tmp/test2.go && gofmt /tmp/test.go > vkslash.go

clean:
	rm -f vkslash.go

test:
	go build
