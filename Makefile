all:
	./header-generator/header-generator
	gofmt -s -w vk_wrapper.go

clean:
	rm -f vk_wrapper.go

test:
	go build
