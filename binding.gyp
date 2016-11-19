{
  "targets": [
    {
      "target_name": "distance",
      "sources": [
        "src/distance.cc"
      ],
      "include_dirs": ["<!(node -e \"require('nan')\")"]
    }
  ]
}