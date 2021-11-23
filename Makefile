all:
	./header-generator/header-generator | gofmt > vkslash.go

clean:
	rm -f vkslash.go

test:
	go build
