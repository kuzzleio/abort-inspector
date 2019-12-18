{
  "targets": [
    {
      "target_name": "AbortInspector",
      "sources": [
        "src/trap.cc"
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
