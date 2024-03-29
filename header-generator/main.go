package main

import (
	"os"

	"gopkg.in/yaml.v3"
)

func main() {
	config := LoadConfig("vkxml.yml")
	for _, v := range config.ExportTranslator {
		switch v.Action {
		case "deprefix":
			TranslatorRules = append(TranslatorRules, exportTranslatorRuleDeprefix{v.Pattern})
		case "title":
			TranslatorRules = append(TranslatorRules, exportTranslatorRuleTitle{})
		}
	}

	enabledMap := make(map[string]bool, 0)
	for _, v := range config.Features {
		enabledMap[v] = true
	}
	for _, v := range config.Extensions {
		enabledMap[v] = true
	}

	// Load up the XML data.
	registry := LoadRegistry(config.VkxmlPath)

	// Trim the registry to the specified API.
	registry = registry.ForApi(config.Api)

	graph, constants := registry.Graph(enabledMap)

	for _, v := range config.Features {
		graph.ApplyFeatureExtensions(v, constants)
	}
	for _, v := range config.Extensions {
		graph.ApplyExtensionExtensions(v, enabledMap, constants)
	}
	graph.DepthFirstSearch(config.Enabled(), CacheConverters)

	if err := GenerateGoFile(config, constants, graph); err != nil {
		panic(err)
	}
	if err := GenerateCDefineFile(config, graph); err != nil {
		panic(err)
	}
	if err := GenerateCImplementFile(config, graph); err != nil {
		panic(err)
	}
}

func OpenOutput(fn string) *os.File {
	fh, err := os.OpenFile(fn, os.O_WRONLY|os.O_TRUNC|os.O_CREATE, 0644)
	if err != nil {
		panic(err)
	}
	return fh
}

func LoadConfig(fn string) *Config {
	fh, err := os.OpenFile(fn, os.O_RDONLY, 0)
	if err != nil {
		panic(err)
	}
	defer fh.Close()

	var config Config
	decoder := yaml.NewDecoder(fh)
	if err := decoder.Decode(&config); err != nil {
		panic(err)
	}

	return &config
}

type Config struct {
	PackageName      string
	VkxmlPath        string
	OutputName       string
	Api              string
	Features         []string
	Extensions       []string
	GlobalProcs      []string
	ProcLoaders      map[string]string
	CDefinePreProc   []string
	ExportTranslator []TranslatorConfig
}

func (config *Config) Enabled() []string {
	enabled := make([]string, 0, len(config.Features)+len(config.Extensions))
	for _, v := range config.Features {
		enabled = append(enabled, v)
	}
	for _, v := range config.Extensions {
		enabled = append(enabled, v)
	}
	return enabled
}

func (config *Config) IsGlobalProc(specName string) bool {
	for _, v := range config.GlobalProcs {
		if v == specName {
			return true
		}
	}
	return false
}

type TranslatorConfig struct {
	Action      string
	Pattern     string
	Replacement string
}

func LoadRegistry(fn string) *Registry {
	// Open the datasource.
	fh, err := os.OpenFile(fn, os.O_RDONLY, 0)
	if err != nil {
		panic(err)
	}
	defer fh.Close()

	if registry, err := DecodeRegistry(fh); err != nil {
		panic(err)
	} else {
		return registry
	}
}
